/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msennane <msennane@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/07 23:39:15 by msennane          #+#    #+#             */
/*   Updated: 2024/12/17 01:50:04 by msennane         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static void process_argument(char *arg, t_queue_char *queue, int *exit_status,
                             t_shell_context *context) {
  int i;
  int *values[2];
  int prev_i;

  if (!arg || !queue || !exit_status || !context)
    return;
  i = 0;
  while (arg[i] != '\0') {
    prev_i = i;
    if (arg[i] == '\'')
      handle_single_quotes(arg, &i, queue);
    else if (arg[i] == '\"') {
      values[0] = &i;
      values[1] = exit_status;
      handle_double_quotes(arg, values, queue, context);
    } else if (arg[i + 1] == '%') {
      // Dynamically handle multiple '%' characters after '$'
      enqueue_char(queue, '$');
      i++;
      while (arg[i] == '%') {
        enqueue_char(queue, '%');
        i++;
      }
    } else if (arg[i] == '$') {
      values[0] = &i;
      values[1] = exit_status;
      handle_dollar_sign(arg, values, queue, context);
    } else
      enqueue_char(queue, arg[i++]);
    if (prev_i == i && arg[i] != '\0')
      i++;
  }
}

static char *clean_argument(char *arg, t_shell_context *context,
                            int *exit_status) {
  t_queue_char queue;
  char *cleaned_arg;
  char *home;

  init_queue_char(&queue);
  if (ft_strcmp(arg, "~") == 0) {
    home = get_env_value("HOME", context->env_vars);
    if (home)
      cleaned_arg = home;
    else
      cleaned_arg = ft_strdup("~");
  } else {
    process_argument(arg, &queue, exit_status, context);
    cleaned_arg = queue_char_str_convert(&queue);
  }
  enqueue(&context->queue, cleaned_arg);
  enqueue(&context->queue, " ");
  return (cleaned_arg);
}

void clean_nulls_from_argv(char **argv, int size) {
  int i;
  int j;
  size_t len;

  i = 0;
  j = 0;
  while (i < size) {
    if (argv[i] != NULL) {
      // Skip arguments that are only special characters
      if (argv[i][0] == '\x01' && argv[i][1] == '\0') {
        i++;
        continue;
      }
      // Remove special character from start
      if (argv[i][0] == '\x01') {
        ft_memmove(argv[i], argv[i] + 1, strlen(argv[i]));
      }
      // Remove all special characters from the end
      len = strlen(argv[i]);
      while (len > 0 && argv[i][len - 1] == '\x01') {
        argv[i][len - 1] = '\0';
        len--;
      }
      argv[j] = argv[i];
      j++;
    }
    i++;
  }
  while (j < size) {
    argv[j] = NULL;
    j++;
  }
}

char *remove_quotes(const char *str) {
  int i;
  int j;
  int in_double_quotes;
  char *result;
  int count;
  int k;

  i = 0;
  j = 0;
  in_double_quotes = 0;
  result = gc_malloc(strlen(str) + 1);
  if (!result)
    return (ft_strdup(""));
  while (str[i] != '\0') {
    if (str[i] == '\"' && !in_double_quotes) {
      in_double_quotes = 1;
      i++;
    } else if (str[i] == '\"' && in_double_quotes) {
      in_double_quotes = 0;
      i++;
    } else if (str[i] == '\'' && !in_double_quotes) {
      // Handle empty single quotes
      if (str[i + 1] == '\'') {
        i += 2; // Skip both quotes
        continue;
      }
      // Check for consecutive single quotes
      count = 0;
      k = i;
      while (str[k] == '\'') {
        count++;
        k++;
      }
      // If we have multiple consecutive quotes followed by content,
      // copy them
      if (count > 1 && str[k] != '\0') {
        while (count > 0) {
          result[j++] = '\'';
          i++;
          count--;
        }
      } else {
        // Regular quote handling
        i++; // Skip opening quote
        while (str[i] && str[i] != '\'') {
          result[j++] = str[i++];
        }
        if (str[i] == '\'')
          i++; // Skip closing quote
      }
    } else {
      result[j++] = str[i++];
    }
  }
  result[j] = '\0';
  return (result);
}

void restore_quotes(char **vector) {
  char *str;

  if (!vector)
    return;
  for (int i = 0; vector[i]; i++) {
    str = vector[i];
    for (int j = 0; str[j]; j++) {
      if (str[j] == '\x1F')
        str[j] = '\'';
    }
  }
}

void clean_execution_command_args(t_command *cmd, t_shell_context *context,
                                  int *exit_status) {
  int i;
  int size;
  t_exec *exec;
  char *res;
  char **vector;

  if (!cmd || !context)
    return;
  exec = (t_exec *)cmd;
  i = 0;
  size = 0;
  // Calculate size only if argv exists
  if (!exec->argv[0])
    return;
  while (exec->argv[size])
    size++;
  while (exec->argv[i]) {
    if (has_special_characters(exec->argv[i]))
      exec->argv[i] = clean_argument(exec->argv[i], context, exit_status);
    else {
      enqueue(&context->queue, exec->argv[i]);
      enqueue(&context->queue, " ");
    }
    i++;
  }
  clean_nulls_from_argv(exec->argv, size);
  res = queue_str_convert(&context->queue);
  // Check if res is not NULL before splitting
  if (!res)
    return;
  vector = ft_split_beta(res, ' ');
  // Add NULL check for vector
  if (!vector)
    return;
  clean_nulls_from_argv(vector, size);
  i = 0;
  while (vector[i]) {
    // printf("vector[%d]: %s\n", i, vector[i]);
    vector[i] = remove_quotes(vector[i]);
    i++;
  }
  restore_quotes(vector);
  i = 0;
  while (vector[i]) {
    // printf("vector[%d]: %s\n", i, vector[i]);
    exec->argv[i] = vector[i];
    i++;
  }
  exec->argv[i] = NULL;
}

void process_all_commands(t_command *cmd, t_shell_context *context,
                          int *exit_status) {
  t_pipe *pipe;
  t_redir *redir;

  if (cmd->type == CMD_EXEC) {
    clean_execution_command_args(cmd, context, exit_status);
  } else if (cmd->type == CMD_PIPE) {
    pipe = (t_pipe *)cmd;
    process_all_commands(pipe->left, context, exit_status);
    process_all_commands(pipe->right, context, exit_status);
  } else if (cmd->type == CMD_REDIR) {
    redir = (t_redir *)cmd;
    if (redir->redir_type != '%' && has_special_characters(redir->file)) {
      redir->file = clean_argument2(redir->file, context, exit_status);
    }
    process_all_commands(redir->sub_cmd, context, exit_status);
  }
}

int count_redirections(t_command *cmd) {
  t_redir *redir;
  int count;

  count = 0;
  if (cmd->type == CMD_REDIR) {
    redir = (t_redir *)cmd;
    if (redir->redir_type == '%')
      count++;
    count += count_redirections(redir->sub_cmd);
  }
  return (count);
}
