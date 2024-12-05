/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_handler2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msennane <msennane@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/01 23:46:29 by msennane          #+#    #+#             */
/*   Updated: 2024/12/05 15:43:49 by msennane         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static void	signal_handler(void)
{
	signal(SIGINT, SIG_IGN);
	signal(SIGQUIT, SIG_IGN);
}

static void	handle_heredoc(pid_t pid1, int fd[2], t_shell_context *context,
		int *exit_status)
{
	int	status;

	waitpid(pid1, &status, 0);
	if (WIFEXITED(status) && WEXITSTATUS(status) != 0)
	{
		ft_close(context, fd[0]);
		ft_close(context, fd[1]);
		*exit_status = WEXITSTATUS(status);
	}
}

static void	handle_pipeline_child_processes(t_pipe *pipe_cmd, int fd[2],
		t_shell_context *context, int *exit_status)
{
	pid_t	pid1;
	int		status;
	int		is_heredoc;

	is_heredoc = 0;
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
		handle_heredoc(pid1, fd, context, exit_status);
}

void	execute_pipeline_command(t_command *cmd, t_shell_context *context,
		int *exit_status)
{
	int		fd[2];
	pid_t	pid2;
	int		status;
	t_pipe	*pipe_cmd;

	pipe_cmd = (t_pipe *)cmd;
	ft_pipe(fd, context);
	handle_pipeline_child_processes(pipe_cmd, fd, context, exit_status);
	pid2 = ft_fork(context);
	if (pid2 == 0)
		right_pipe(pipe_cmd->right, context, fd, exit_status);
	signal_handler();
	ft_close(context, fd[0]);
	ft_close(context, fd[1]);
	waitpid(pid2, &status, 0);
	if (WIFEXITED(status))
		*exit_status = WEXITSTATUS(status);
	else
		*exit_status = 1;
	save_exit_status(context, *exit_status);
	terminate_cleanly(context, *exit_status);
}
