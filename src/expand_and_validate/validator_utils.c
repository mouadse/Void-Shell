/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   validator_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msennane <msennane@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/07 23:38:44 by msennane          #+#    #+#             */
/*   Updated: 2024/11/08 00:19:22 by msennane         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int check_redirection_syntax(char **ps, char *es, int *exit_status) {
  char *q;
  char *eq;
  int ret;
  ret = gettoken(ps, es, &q, &eq);
  if (ret == '\0') {
    ft_putstr_fd("Void-shell: syntax error near unexpected token `newline'\n",
                 STDERR_FILENO);
    *exit_status = 258;
    return (0);
  } else if (ret != 'a') {
    *eq = '\0';
    ft_putstr_fd("Void-shell: syntax error near unexpected token `",
                 STDERR_FILENO);
    ft_putstr_fd(q, STDERR_FILENO);
    ft_putstr_fd("'\n", STDERR_FILENO);
    *exit_status = 258;
    return (0);
  }
  return (1);
}

int validate_pipe_syntax(char **ps, char *es, int *exit_status) {
  if (peek(ps, es, "|")) {
    ft_putstr_fd("Void-shell: syntax error near unexpected token '|'\n",
                 STDERR_FILENO);
    *exit_status = 258;
    return (0);
  }
  return (1);
}

int check_pipe_usage(char **ps, char *es, int *exit_status) {
  int ret;

  ret = gettoken(ps, es, 0, 0);
  if (ret == '|' || ret == '\0') {
    ft_putstr_fd("Void-shell: syntax error near unexpected token '|'\n",
                 STDERR_FILENO);
    *exit_status = 258;
    return (0);
  }
  return (1);
}
