/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msennane <msennane@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/24 22:01:30 by msennane          #+#    #+#             */
/*   Updated: 2024/12/02 12:15:50 by msennane         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef UTILS_H
# define UTILS_H

# include "command.h"
# include <sys/types.h>

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

void	retrieve_exit_status(t_command *cmd, t_shell_context *context,
			int *exit_status, int status);
void	store_subprocess_pid(pid_t pid, t_shell_context *context);
int		is_built_in_command(t_command *cmd);
void	run_built_in_command(t_exec *cmd, t_env_var **env_list,
			int *exit_status);
void	clean_shell(t_shell_context *context);

#endif
