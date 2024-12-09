/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msennane <msennane@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/07 23:39:15 by msennane          #+#    #+#             */
/*   Updated: 2024/12/10 00:35:44 by msennane         ###   ########.fr       */
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

    if (arg[i] == '\'') {
      handle_single_quotes(arg, &i, queue);
    } else if (arg[i] == '$') {
      if (!arg[i + 1] || is_whitespace(arg[i + 1]) || arg[i + 1] == '\"') {
        enqueue_char(queue, '$');
        i++;
      } else {
        values[0] = &i;
        values[1] = exit_status;
        handle_dollar_sign(arg, values, queue, context);
      }
    } else {
      enqueue_char(queue, arg[i++]);
    }
    // Prevent infinite loop
    if (prev_i == i && arg[i] != '\0') {
      i++;
    }
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

// addition here
void clean_nulls_from_argv(char **argv, int size) {
  int i;
  int j;

  i = 0;
  j = 0;
  while (i < size) {
    if (argv[i] != NULL && argv[i][0] != '\x01') {
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
  int i = 0, j = 0;
  int in_double_quotes = 0;
  int in_single_quotes = 0;
  char *result = gc_malloc(strlen(str) + 1);

  if (!result)
    return ft_strdup("");

  while (str[i] != '\0') {
    if (str[i] == '\"' && !in_single_quotes) {
      // Handle double quotes when not in single quotes
      in_double_quotes = !in_double_quotes;
      i++;
    } else if (str[i] == '\'' && !in_double_quotes) {
      // Handle single quotes when not in double quotes
      in_single_quotes = !in_single_quotes;
      i++;
    } else {
      // Keep all characters inside quotes
      result[j++] = str[i++];
    }
  }
  result[j] = '\0';
  return result;
}

static void clean_execution_command_args(t_command *cmd,
                                         t_shell_context *context,
                                         int *exit_status) {
  int i;
  int size;
  t_exec *exec;

  exec = (t_exec *)cmd;
  i = 0;
  size = 0;
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
  char *res = queue_str_convert(&context->queue);
  char **vector = ft_split_beta(res, ' ');
  clean_nulls_from_argv(vector, size);
  for (int i = 0; vector[i]; i++) {
    vector[i] = remove_quotes(vector[i]);
  }
  i = 0;
  while (vector[i]) {
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
    if (redir->redir_type != '%' && has_special_characters(redir->file))
      redir->file = clean_argument(redir->file, context, exit_status);
    process_all_commands(redir->sub_cmd, context, exit_status);
  }
}
