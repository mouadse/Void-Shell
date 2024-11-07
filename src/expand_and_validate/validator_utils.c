/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   validator_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msennane <msennane@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/07 23:38:44 by msennane          #+#    #+#             */
/*   Updated: 2024/11/07 23:38:54 by msennane         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int validate_pipe_syntax(char **ps, char *es, int *exit_status) {
  if (peek(ps, es, "|")) {
    ft_putstr_fd("syntax error near unexpected token '|'\n", STDERR_FILENO);
    *exit_status = 258;
    return (0);
  }
  return (1);
}
