/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_handler.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msennane <msennane@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/01 23:46:29 by msennane          #+#    #+#             */
/*   Updated: 2024/11/01 23:47:05 by msennane         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	execute_pipeline_command(t_command *cmd, t_shell_context *context,
		int *exit_status)
{
	int		fd[2];
	pid_t	left_child;
	pid_t	right_child;
	int		status;
	int		heredoc_flag;
	t_pipe	*pipe_cmd;

	pipe_cmd = (t_pipe *)cmd;
	if (pipe(fd) < 0)
		perror("Pipe");
	left_child = fork();
	if (!left_child)
	{
		close(fd[0]);
		dup2(fd[1], STDOUT_FILENO);
		close(fd[1]);
		// run_cmd
		exit(EXIT_SUCCESS);
	}
	if (pipe_cmd->left->type == CMD_REDIR
		&& ((t_redir *)pipe_cmd->left)->redir_type == '%')
	{
		close(fd[0]);
		close(fd[1]);
		waitpid(left_child, NULL, 0);
		heredoc_flag = 1;
	}
	right_child = fork();
	if (!right_child)
	{
		close(fd[1]);
		dup2(fd[0], STDIN_FILENO);
		close(fd[0]);
		// run_cmd
		exit(EXIT_SUCCESS);
	}
	if (!heredoc_flag)
	{
		close(fd[0]);
		close(fd[1]);
		waitpid(left_child, NULL, 0);
	}
	waitpid(right_child, &status, 0);
	if (WIFEXITED(status))
		*exit_status = WEXITSTATUS(status);
	else
		*exit_status = EXIT_FAILURE;
	// Save the exit status somewhere
	// free and exit
}
