/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander_filename.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msennane <msennane@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/07 23:39:15 by msennane          #+#    #+#             */
/*   Updated: 2024/12/11 21:37:53 by msennane         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static void handle_single_quotes2(char *str, int *index, t_queue_char *queue) {
  if (!str || !index || !queue)
    return;

  (*index)++;

  while (str[*index] && str[*index] != '\'') {
    enqueue_char(queue, str[*index]);
    (*index)++;
  }

  if (str[*index] == '\'') {
    (*index)++;
  }
}

static void handle_double_quotes2(char *arg, int *values[2], t_queue_char *q,
                                  t_shell_context *context) {
  if (!arg || !values || !q || !context)
    return;

  int *i = values[0];
  int *exit_status = values[1];
  (*i)++;
  while (arg[*i] && arg[*i] != '\"') {
    if (arg[*i] == '$') {
      if (!arg[*i + 1] || is_whitespace(arg[*i + 1]) || arg[*i + 1] == '\"') {
        enqueue_char(q, '$');
        (*i)++;
      } else if (arg[*i + 1] == '?') {
        char *exit_status_str = ft_itoa(*exit_status);
        if (exit_status_str) {
          enqueue_str(q, exit_status_str);
          (*i) += 2;
        }
      } else {
        (*i)++;
        char *var_name = extract_variable_name(arg + *i);
        if (var_name) {
          char *var_value = get_env_value(var_name, context->env_vars);
          if (var_value)
            enqueue_str(q, var_value);
          (*i) += ft_strlen(var_name);
        }
      }
    } else {
      enqueue_char(q, arg[*i]);
      (*i)++;
    }
  }
  if (arg[*i] == '\"')
    (*i)++;
}

static void process_argument2(char *arg, t_queue_char *queue, int *exit_status,
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
      handle_single_quotes2(arg, &i, queue);
    else if (arg[i] == '\"') {
      values[0] = &i;
      values[1] = exit_status;
      handle_double_quotes2(arg, values, queue, context);
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

char *clean_argument2(char *arg, t_shell_context *context, int *exit_status) {
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
    process_argument2(arg, &queue, exit_status, context);
    cleaned_arg = queue_char_str_convert(&queue);
  }
  return (cleaned_arg);
}

