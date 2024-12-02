/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution_manager.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msennane <msennane@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/01 23:46:23 by msennane          #+#    #+#             */
/*   Updated: 2024/12/02 13:51:04 by msennane         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"
#include <stdio.h>

void	print_exec_error(char *cmd_name, char *error_type)
{
	ft_putstr_fd("Void-shell: ", STDERR_FILENO);
	ft_putstr_fd(cmd_name, STDERR_FILENO);
	ft_putstr_fd(": ", STDERR_FILENO);
	ft_putstr_fd(error_type, STDERR_FILENO);
	ft_putstr_fd("\n", STDERR_FILENO);
}

static int	file_exists(const char *path)
{
	struct stat	buffer;

	return (stat(path, &buffer) == 0);
}

static int	is_executable(const char *path)
{
	return (access(path, X_OK) == 0);
}

static int	is_builtin_command(t_exec *exec_cmd)
{
	const char	*builtins[] = {"echo", "exit", "env", "export", "unset", "pwd"};
	size_t		i;
	size_t		builtin_count;

	builtin_count = sizeof(builtins) / sizeof(builtins[0]);
	i = 0;
	while (i < builtin_count)
	{
		if (ft_strcmp(exec_cmd->argv[0], builtins[i]) == 0)
			return (1);
		i++;
	}
	return (0);
}

static void	execute_builtin_command(t_exec *exec_cmd, t_shell_context *context,
		int exit_status)
{
	(void)exit_status;
	(void)context; // to be updated later
	if (ft_strcmp(exec_cmd->argv[0], "echo") == 0)
		printf("Executing builtin command: echo\n");
	else if (ft_strcmp(exec_cmd->argv[0], "exit") == 0)
		printf("Executing builtin command: exit\n");
	else if (ft_strcmp(exec_cmd->argv[0], "env") == 0)
		printf("Executing builtin command: env\n");
	else if (ft_strcmp(exec_cmd->argv[0], "export") == 0)
		printf("Executing builtin command: export\n");
	else if (ft_strcmp(exec_cmd->argv[0], "unset") == 0)
		printf("Executing builtin command: unset\n");
	else if (ft_strcmp(exec_cmd->argv[0], "pwd") == 0)
		printf("Executing builtin command: pwd\n");
}

static void	execute_external_command(t_command *cmd, t_shell_context *context)
{
	t_exec	*exec_cmd;
	char	*binary_path;

	exec_cmd = (t_exec *)cmd;
	/* Attempt to find the full path of the command */
	binary_path = get_command_path(exec_cmd->argv[0], context->env_vars);
	if (!binary_path)
	{
		/* Command not found in PATH */
		print_exec_error(exec_cmd->argv[0], "command not found");
		terminate_cleanly(context, 127);
	}
	/* Pre-execution checks */
	if (!file_exists(binary_path))
	{
		print_exec_error(exec_cmd->argv[0], "No such file or directory");
		ft_free(binary_path);
		terminate_cleanly(context, 127);
	}
	if (!is_executable(binary_path))
	{
		print_exec_error(exec_cmd->argv[0], "Permission denied");
		ft_free(binary_path);
		terminate_cleanly(context, 127);
	}
	/* Attempt to execute the command */
	handle_execve(binary_path, exec_cmd->argv, context->envp, context);
}

void	run_exec(t_command *cmd, t_shell_context *context, int *exit_status)
{
	t_exec	*exec_cmd;

	exec_cmd = (t_exec *)cmd;
	clean_empty_arguments(exec_cmd);
	handle_executable_path(exec_cmd, context);
	if (is_builtin_command(exec_cmd))
	{
		execute_builtin_command(exec_cmd, context, exit_status);
		// Do not terminate here; allow the shell to continue running
	}
	else
	{
		if (access(exec_cmd->argv[0], X_OK) == 0)
		{
			execve(exec_cmd->argv[0], exec_cmd->argv, context->envp);
			print_exec_error(exec_cmd->argv[0], "Error executing command");
			terminate_cleanly(context, 127);
		}
		else
		{
			execute_external_command(cmd, context);
		}
	}
}
