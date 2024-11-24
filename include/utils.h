/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msennane <msennane@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/24 22:01:30 by msennane          #+#    #+#             */
/*   Updated: 2024/11/24 22:30:26 by msennane         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef UTILS_H
# define UTILS_H

# include "command.h"

void	ft_free(void *ptr);
void	terminate_cleanly(t_shell_context *context, int status);
void	terminate_with_error(t_shell_context *context, char *message,
			int status);
void	free_array(char **array);
int		is_whitespace(char c);
int		is_whitespace_string(char *str);
int		is_numeric(const char *str);
int		ft_fork(t_shell_context *context);
void	ft_pipe(int fd[2], t_shell_context *context);

#endif
