/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msennane <msennane@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/07 23:39:28 by msennane          #+#    #+#             */
/*   Updated: 2024/12/16 00:47:12 by msennane         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"
#include <stdio.h>
int has_special_characters(char *str) {
  if (!str)
    return 0;
  return (ft_strchr(str, '\'') || ft_strchr(str, '\"') || ft_strchr(str, '$') ||
          ft_strchr(str, '~') || ft_strchr(str, '\\'));
}
void handle_single_quotes(char *str, int *index, t_queue_char *queue) {
  if (!str || !index || !queue)
    return;

  enqueue_char(queue, '\''); // Keep the opening quote
  (*index)++;

  while (str[*index] && str[*index] != '\'') {
    enqueue_char(queue, str[*index]);
    (*index)++;
  }

  if (str[*index] == '\'') {
    enqueue_char(queue, '\''); // Keep the closing quote
    (*index)++;
  }
}

// char *extract_variable_name(char *arg) {
//   if (!arg)
//     return NULL;
//   int i = 0;

//   if (ft_isdigit(arg[i])) {
//     int n = arg[i] - '0';
//     return ft_itoa(n);
//   }

//   while (arg[i] && (ft_isalnum(arg[i]) || arg[i] == '_'))
//     i++;
//   return ft_substr(arg, 0, i);
// }

char *extract_variable_name(char *arg) {
  if (!arg)
    return NULL;

  // Handle special parameters first
  if (arg[0] &&
      (arg[0] == '@' || arg[0] == '*' || arg[0] == '#' || arg[0] == '?' ||
       arg[0] == '-' || arg[0] == '$' || arg[0] == '!' || ft_isdigit(arg[0]))) {
    return ft_substr(arg, 0, 1);
  }

  int i = 0;

  // For regular variables, first character must be letter or underscore
  if (arg[i] && (ft_isalpha(arg[i]) || arg[i] == '_')) {
    i++;
    // Subsequent characters can be alphanumeric or underscore
    while (arg[i] && (ft_isalnum(arg[i]) || arg[i] == '_'))
      i++;
  }

  // If no valid characters found, return empty string
  if (i == 0)
    return ft_strdup("");

  return ft_substr(arg, 0, i);
}

void process_variable(char *str, int *values[2], t_queue_char *queue,
                      t_shell_context *context) {
  if (!str || !values || !queue || !context)
    return;

  int *i = values[0];
  char *var_name = extract_variable_name(str + *i);
//   printf("var_name: %s\n", var_name);
  if (!var_name) {
    // printf("var_name is NULL\n");
    // enqueue_char(queue, '$');
    // (*i)++;
    return;
  }
  char *var_value = get_env_value(var_name, context->env_vars);
  if (var_value)
    enqueue_str(queue, var_value);
  else
    enqueue_char(queue, '\x01');
  (*i) += ft_strlen(var_name);
}

void handle_dollar_sign(char *str, int *values[2], t_queue_char *queue,
                        t_shell_context *context) {
  if (!str || !values || !queue || !context)
    return;

  int *index = values[0];
  int *exit_status = values[1];

  (*index)++;
  if (!str[*index] || is_whitespace(str[*index])) {
    enqueue_char(queue, '$');
    return;
  }

  if (str[*index] == '?') {
    char *exit_status_str = ft_itoa(*exit_status);
    if (exit_status_str) {
      enqueue_str(queue, exit_status_str);
      (*index)++;
    }
  } else {
    process_variable(str, values, queue, context);
  }
}
void handle_double_quotes(char *arg, int *values[2], t_queue_char *q,
                          t_shell_context *context) {
  if (!arg || !values || !q || !context)
    return;

  int *i = values[0];
  int *exit_status = values[1];
  enqueue_char(q, '\"');
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
  if (arg[*i] == '\"') {
    enqueue_char(q, '\"');
    (*i)++;
  }
}
