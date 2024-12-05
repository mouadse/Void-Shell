/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_handler.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msennane <msennane@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/01 23:46:29 by msennane          #+#    #+#             */
/*   Updated: 2024/12/05 13:04:04 by msennane         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static void	save_exit_status(t_shell_context *context, int status_code)
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

static void	left_pipe(t_shell_context *context, t_command *cmd, int fd[2],
		int *exit_status)
{
	ft_close(context, fd[0]);
	dup2(fd[1], STDOUT_FILENO);
	ft_close(context, fd[1]);
	execute_command(cmd, context, exit_status);
	exit(0);
}

static void	right_pipe(t_command *cmd, t_shell_context *context, int fd[2],
		int *exit_status)
{
	ft_close(context, fd[1]);
	dup2(fd[0], STDIN_FILENO);
	ft_close(context, fd[0]);
	execute_command(cmd, context, exit_status);
	exit(0);
}

static void	signal_handler(void)
{
	signal(SIGINT, SIG_IGN);
	signal(SIGQUIT, SIG_IGN);
}

void	execute_pipeline_command(t_command *cmd, t_shell_context *context,
		int *exit_status)
{
	int		fd[2];
	pid_t	pid1;
	pid_t	pid2;
	int		status;
	t_pipe	*pipe_cmd;
	int		is_heredoc;

	is_heredoc = 0;
	pipe_cmd = (t_pipe *)cmd;
	ft_pipe(fd, context);
	pid1 = ft_fork(context);
	if (pid1 == 0)
	{
		if (pipe_cmd->left->type == CMD_REDIR
			&& ((t_redir *)pipe_cmd->left)->redir_type == '%')
		{
			ft_close(context, fd[0]);
			dup2(fd[1], STDOUT_FILENO);
			ft_close(context, fd[1]);
			execute_command(pipe_cmd->left, context, exit_status);
		}
		else
			left_pipe(context, pipe_cmd->left, fd, exit_status);
	}
	if (pipe_cmd->left->type == CMD_REDIR
		&& ((t_redir *)pipe_cmd->left)->redir_type == '%')
	{
		is_heredoc = 1;
		waitpid(pid1, &status, 0);
		if (WIFEXITED(status) && WEXITSTATUS(status) != 0)
		{
			ft_close(context, fd[0]);
			ft_close(context, fd[1]);
			*exit_status = WEXITSTATUS(status);
		}
	}
	pid2 = ft_fork(context);
	if (pid2 == 0)
		right_pipe(pipe_cmd->right, context, fd, exit_status);
	signal_handler();
	ft_close(context, fd[0]);
	ft_close(context, fd[1]);
	if (!is_heredoc)
		waitpid(pid1, &status, 0);
	waitpid(pid2, &status, 0);
	if (WIFEXITED(status))
		*exit_status = WEXITSTATUS(status);
	else
		*exit_status = 1;
	save_exit_status(context, *exit_status);
	terminate_cleanly(context, *exit_status);
}
