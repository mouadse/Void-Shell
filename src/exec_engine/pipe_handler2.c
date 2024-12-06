/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_handler2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msennane <msennane@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/01 23:46:29 by msennane          #+#    #+#             */
/*   Updated: 2024/12/06 18:55:48 by msennane         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	save_exit_status(t_shell_context *context, int status_code)
{
	int		fd;
	ssize_t	bytes_written;

	fd = open(SHELL_CHILD_STATUS_FILE, O_WRONLY | O_CREAT | O_APPEND, 0777);
	if (fd < 0)
	{
		perror("open");
		terminate_with_error(context, "Open", EXIT_FAILURE);
	}
	bytes_written = write(fd, &status_code, sizeof(status_code));
	if (bytes_written < 0)
	{
		perror("write");
		terminate_with_error(context, "Write", EXIT_FAILURE);
	}
	ft_close(context, fd);
}

void	left_pipe(t_shell_context *context, t_command *cmd, int fd[2],
		int *exit_status)
{
	ft_close(context, fd[0]);
	if (dup2(fd[1], STDOUT_FILENO) == -1)
	{
		perror("dup2");
		exit(EXIT_FAILURE);
	}
	ft_close(context, fd[1]);
	execute_command(cmd, context, exit_status);
	exit(0);
}

void	right_pipe(t_command *cmd, t_shell_context *context, int fd[2],
		int *exit_status)
{
	ft_close(context, fd[1]);
	if (dup2(fd[0], STDIN_FILENO) == -1)
	{
		perror("dup2");
		exit(EXIT_FAILURE);
	}
	ft_close(context, fd[0]);
	execute_command(cmd, context, exit_status);
	exit(0);
}
