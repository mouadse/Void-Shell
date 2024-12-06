/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_handler.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msennane <msennane@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/01 23:46:29 by msennane          #+#    #+#             */
/*   Updated: 2024/12/06 13:39:40 by msennane         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

// void	save_exit_status(t_shell_context *context, int status_code)
// {
// 	int		fd;
// 	ssize_t	bytes_written;

// 	fd = open(SHELL_CHILD_STATUS_FILE, O_WRONLY | O_CREAT | O_APPEND, 0777);
// 	if (fd < 0)
// 	{
// 		perror("open");
// 		terminate_with_error(context, "Open", EXIT_FAILURE);
// 	}
// 	bytes_written = write(fd, &status_code, sizeof(status_code));
// 	if (bytes_written < 0)
// 	{
// 		perror("write");
// 		terminate_with_error(context, "Write", EXIT_FAILURE);
// 	}
// 	ft_close(context, fd);
// }

// void	left_pipe(t_shell_context *context, t_command *cmd, int fd[2],
// 		int *exit_status)
// {
// 	ft_close(context, fd[0]);
// 	dup2(fd[1], STDOUT_FILENO);
// 	ft_close(context, fd[1]);
// 	execute_command(cmd, context, exit_status);
// 	exit(0);
// }

// void	right_pipe(t_command *cmd, t_shell_context *context, int fd[2],
// 		int *exit_status)
// {
// 	ft_close(context, fd[1]);
// 	dup2(fd[0], STDIN_FILENO);
// 	ft_close(context, fd[0]);
// 	execute_command(cmd, context, exit_status);
// 	exit(0);
// }

// Executes the left subtree of the pipeline
static void	execute_left_subtree(t_command *cmd, int fd[2],
		t_shell_context *context, int *exit_status)
{
	// Redirect stdout to the write end of the pipe
	close(fd[0]);
	if (dup2(fd[1], STDOUT_FILENO) == -1)
	{
		perror("dup2");
		terminate_with_error(context, "dup2", 1);
	}
	close(fd[1]);
	// Execute the left command
	run_exec(cmd, context, exit_status);
	// Exit the child process
	exit(0);
}

// Executes the right subtree of the pipeline
static void	execute_right_subtree(t_command *cmd, int fd[2],
		t_shell_context *context, int *exit_status)
{
	// Redirect stdin to the read end of the pipe
	close(fd[1]);
	if (dup2(fd[0], STDIN_FILENO) == -1)
	{
		perror("dup2");
		terminate_with_error(context, "dup2", 1);
	}
	close(fd[0]);
	// Execute the right command
	run_exec(cmd, context, exit_status);
	// Exit the child process
	exit(0);
}

// Writes the exit status to a temporary file
static void	write_exit_status_to_file(t_shell_context *context, int exit_status)
{
	int	fd;

	fd = open(SHELL_CHILD_STATUS_FILE, O_WRONLY | O_CREAT | O_APPEND, 0777);
	if (fd < 0)
	{
		perror("open");
		terminate_with_error(context, "open", 1);
	}
	if (write(fd, &exit_status, sizeof(int)) < 0)
	{
		perror("write");
		close(fd);
		terminate_with_error(context, "write", 1);
	}
	close(fd);
}

// Sets up signal handlers to ignore SIGINT and SIGQUIT
static void	set_signals(void)
{
	if (signal(SIGINT, SIG_IGN) == SIG_ERR)
	{
		perror("signal");
		exit(EXIT_FAILURE);
	}
	if (signal(SIGQUIT, SIG_IGN) == SIG_ERR)
	{
		perror("signal");
		exit(EXIT_FAILURE);
	}
}

// Executes a pipeline command
void	execute_pipeline_command(t_command *cmd, t_shell_context *context,
		int *exit_status)
{
	int		fd[2];
	pid_t	pid1;
	pid_t	pid2;
	int		status;
	int		is_herdoc;
	t_pipe	*pcmd;

	is_herdoc = 0;
	pcmd = (t_pipe *)cmd;
	// Create a pipe
	if (pipe(fd) == -1)
	{
		perror("pipe");
		terminate_with_error(context, "pipe", 1);
	}
	// Fork the first child process for the left subtree
	pid1 = fork();
	if (pid1 < 0)
	{
		perror("fork");
		terminate_with_error(context, "fork", 1);
	}
	if (pid1 == 0)
	{
		// Child process executes the left subtree
		execute_left_subtree(pcmd->left, fd, context, exit_status);
	}
	// Check if the left command is a heredoc
	if (pcmd->left->type == CMD_REDIR
		&& ((t_redir *)pcmd->left)->redir_type == '%')
	{
		// Close both ends of the pipe in the parent
		// close(fd[0]);
		// close(fd[1]);
		// Wait for the first child process
		if (waitpid(pid1, NULL, 0) == -1)
		{
			perror("waitpid");
			terminate_with_error(context, "waitpid", 1);
		}
		is_herdoc = 1;
	}
	// Fork the second child process for the right subtree
	pid2 = fork();
	if (pid2 < 0)
	{
		perror("fork");
		terminate_with_error(context, "fork", 1);
	}
	if (pid2 == 0)
	{
		// Child process executes the right subtree
		execute_right_subtree(pcmd->right, fd, context, exit_status);
	}
	// Set up signal handlers in the parent
	set_signals();
	if (!is_herdoc)
	{
		// Close both ends of the pipe in the parent if not heredoc
		close(fd[0]);
		close(fd[1]);
		// Wait for the first child process
		if (waitpid(pid1, NULL, 0) == -1)
		{
			perror("waitpid");
			terminate_with_error(context, "waitpid", 1);
		}
	}
	// Wait for the second child process
	if (waitpid(pid2, &status, 0) == -1)
	{
		perror("waitpid");
		terminate_with_error(context, "waitpid", 1);
	}
	// Update the exit status based on the second child process
	if (WIFEXITED(status))
		*exit_status = WEXITSTATUS(status);
	else
		*exit_status = 1;
	// Write the exit status to a temporary file
	write_exit_status_to_file(context, *exit_status);
	// Clean up and exit the shell with the appropriate status
	terminate_cleanly(context, *exit_status);
}
