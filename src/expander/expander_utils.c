/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msennane <msennane@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/07 23:39:28 by msennane          #+#    #+#             */
/*   Updated: 2024/11/09 21:24:56 by msennane         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int has_special_characters(char *str) {
  if (ft_strchr(str, '\'') || ft_strchr(str, '\"') || ft_strchr(str, '$') ||
      ft_strchr(str, '~'))
    return (1);
  return (0);
}

void handle_single_quotes(char *str, int *index, t_queue_char *queue) {
  (*index)++; // skip the single quote
  while (str[*index] && str[*index] != '\'') {
    enqueue_char(queue, str[*index]);
    (*index)++;
  }
  (*index)++; // skip the single quote
}
// this function is another helper function that is used to extract the variable
// name
char *extract_variable_name(char *arg) {
  int i;
  int n;

  i = 0;
  if (ft_isdigit(arg[i])) {
    n = arg[i] - '0'; // extract variable name like $1, $2, $3, ...
    return (ft_itoa(n));
  }
  while (arg[i] && ((ft_isalnum(arg[i]) || arg[i] == '_')))
    // extract variable name like $HOME, $USER, ...
    i++;
  return (ft_substr(arg, 0, i));
}

void process_variable(char *str, int *values[2], t_queue_char *queue,
                      t_shell_context *context) {
  char *var_name;
  char *var_value;
  int *i;

  i = values[0];
  var_name = extract_variable_name(str + *i);
  var_value = get_env_value(var_name, context->env_vars);
  if (var_value)
    enqueue_str(queue, var_value);
  (*i) += ft_strlen(var_name);
  free(var_name); // to be replaced by custom free
}

void handle_dollar_sign(char *str, int *values[2], t_queue_char *queue,
                        t_shell_context *context) {
  int *exit_status;
  int *index;
  char *exit_status_str;

  index = values[0];
  exit_status = values[1];
  (*index)++; // skip the dollar sign
  if (!str[*index] || is_whitespace(str[*index])) {
    enqueue_char(queue, '$');
    return;
  } else if (str[*index] == '?') {
    exit_status_str = ft_itoa(*exit_status);
    enqueue_str(queue, exit_status_str);
    free(exit_status_str);
    (*index)++;
  } else
    process_variable(str, values, queue, context);
}

void handle_double_quotes(char *arg, int *values[2], t_queue_char *q, t_shell_context *context) {
    int *i = values[0];
    int *exit_status = values[1];
    char *var_name;
    char *var_value;
    char *exit_status_str;

    (*i)++; // skip the opening double quote
    while (arg[*i] && arg[*i] != '\"') {
        if (arg[*i] == '$' && (is_whitespace(arg[*i + 1]) || arg[*i + 1] == '\"')) {
            enqueue_char(q, '$');
            (*i)++;
        } else if (arg[*i] == '$' && arg[*i + 1] == '?') {
            // process_double_quote_3 functionality
            exit_status_str = ft_itoa(*exit_status);
            enqueue_str(q, exit_status_str);
            free(exit_status_str);
            (*i) += 2; // skip "$?"
        } else if (arg[*i] == '$') {
            // process_double_quote_2 functionality
            (*i)++; // skip the dollar sign
            var_name = extract_variable_name(arg + *i);
            var_value = get_env_value(var_name, context->env_vars);
            if (var_value)
                enqueue_str(q, var_value);
            (*i) += ft_strlen(var_name);
            free(var_name);
        } else {
            enqueue_char(q, arg[(*i)++]);
        }
    }
    (*i)++; // skip the closing double quote
}