/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   void-shell-utils1.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msennane <msennane@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/21 18:39:02 by msennane          #+#    #+#             */
/*   Updated: 2024/12/04 19:46:50 by msennane         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"
#include <stdio.h>

void	retrieve_exit_status(t_command *cmd, t_shell_context *context,
		int *exit_status, int status)
{
	int	fd;

	if (cmd && (cmd->type == CMD_EXEC || cmd->type == CMD_REDIR))
	{
		if (WIFEXITED(status))
			*exit_status = WEXITSTATUS(status);
		else if (WIFSIGNALED(status))
			*exit_status = WTERMSIG(status) + 128;
		else
			*exit_status = EXIT_FAILURE;
	}
	else
	{
		fd = open(SHELL_CHILD_STATUS_FILE, O_RDONLY);
		if (-1 == fd)
			terminate_with_error(context, "open", 1);
		if (-1 == read(fd, exit_status, sizeof(int)))
			terminate_with_error(context, "read", 1);
		ft_close(context, fd);
	}
}

void	store_subprocess_pid(pid_t pid, t_shell_context *context)
{
	int	fd;

	fd = open(SHELL_CHILD_PID_FILE, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (-1 == fd)
		terminate_with_error(context, "open", 1);
	if (-1 == write(fd, &pid, sizeof(pid_t)))
		terminate_with_error(context, "write", 1);
	ft_close(context, fd);
}

int	is_built_in_command(t_command *cmd)
{
	t_exec	*exec_cmd;

	if (!cmd)
		return (0);
	if (cmd && (cmd->type == CMD_EXEC))
	{
		exec_cmd = (t_exec *)cmd;
		if (exec_cmd->argv[0] && (ft_strcmp(exec_cmd->argv[0], "cd") == 0
				|| ft_strcmp(exec_cmd->argv[0], "export") == 0
				|| ft_strcmp(exec_cmd->argv[0], "unset") == 0))
			return (1);
	}
	return (0);
}

void	run_built_in_command(t_exec *cmd, t_env_var **env_list,
		int *exit_status)
{
	(void)env_list;
	if (ft_strcmp(cmd->argv[0], "cd") == 0)
		cd(cmd->argv, exit_status, env_list);
	else if (ft_strcmp(cmd->argv[0], "export") == 0)
	{
		ft_export(cmd->argv, env_list, exit_status);
	}
	else if (ft_strcmp(cmd->argv[0], "unset") == 0)
	{
		// run_unset(cmd, env_var_list, exit_status);
	}
}

void	clean_shell(t_shell_context *context)
{
	// ft_free(context->input);
	// release_command_resources(context->tree);
	// free_queue(&context->queue);
	unlink(SHELL_CHILD_PID_FILE);
	unlink(SHELL_CHILD_STATUS_FILE);
}

void	ft_close(t_shell_context *context, int fd)
{
	if (-1 == close(fd))
		terminate_with_error(context, "close", 1);
}
