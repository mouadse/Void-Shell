/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_handler.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msennane <msennane@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/01 23:46:29 by msennane          #+#    #+#             */
/*   Updated: 2024/11/04 17:44:37 by msennane         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"
#include <fcntl.h>
#include <time.h>

static void	save_exit_status(t_command *cmd, int status_code)
{
	int		fd;
	ssize_t	bytes_written;

	fd = open(SHELL_CHILD_STATUS_FILE, O_WRONLY | O_CREAT | O_APPEND, 0777);
	if (fd < 0)
	{
		perror("open");
		exit(EXIT_FAILURE); // better cleanup function to be added here
	}
	bytes_written = write(fd, &status_code, sizeof(status_code));
	if (bytes_written < 0)
	{
		perror("write");
		exit(EXIT_FAILURE); // better cleanup function to be added here
	}
	close(fd);
}

static void	left_pipe(t_command *cmd, int *fd, int *exit_status)
{
	close(fd[0]);
	dup2(fd[1], STDOUT_FILENO);
	close(fd[1]);
	//   execute_command(cmd, exit_status);
	exit(EXIT_SUCCESS);
}

static void	right_pipe(t_command *cmd, int *fd, int *exit_status)
{
	close(fd[1]);
	dup2(fd[0], STDIN_FILENO);
	close(fd[0]);
	//   execute_command(cmd, exit_status);
	exit(EXIT_SUCCESS);
}

static void	signal_handler(void)
{
	signal(SIGINT, SIG_IGN);
	signal(SIGQUIT, SIG_IGN);
}

void	execute_pipeline_command(t_command *cmd, t_shell_context *context,
		int *exit_status)
{
	int			fd[2];
	pid_t		pids[2];
	int			status;
	static int	heredoc_flag = 0;
	t_pipe		*pipe_cmd;

	pipe_cmd = (t_pipe *)cmd;
	pipe(fd);
	pids[0] = fork(); // for left command
	if (pids[0] < 0)
	{
		perror("fork");
		exit(EXIT_FAILURE); // to be handled better for potential cleanup
	}
	if (!pids[0])
		left_pipe(pipe_cmd->left, fd, exit_status);
	if (pipe_cmd->right->type == CMD_REDIR
		&& ((t_redir *)pipe_cmd->left)->redir_type == '%')
	{
		heredoc_flag = 1;
		close(fd[0]);
		close(fd[1]);
		waitpid(pids[0], NULL, 0);
	}
	pids[1] = fork(); // for right command
	if (pids[1] < 0)
	{
		perror("fork");
		exit(EXIT_FAILURE); // to be handled better for potential cleanup
	}
	if (!pids[1])
		right_pipe(pipe_cmd->right, fd, exit_status);
	signal_handler();
	if (!heredoc_flag)
	{
		close(fd[0]);
		close(fd[1]);
		waitpid(pids[0], NULL, 0);
	}
	waitpid(pids[1], &status, 0);
	if (WIFEXITED(status))
		*exit_status = WEXITSTATUS(status);
	else
		*exit_status = 1;
	save_exit_status(cmd, *exit_status);
	// free and exit our child processes
}
