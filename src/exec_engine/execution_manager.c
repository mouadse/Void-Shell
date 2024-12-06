/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution_manager.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msennane <msennane@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/01 23:46:23 by msennane          #+#    #+#             */
/*   Updated: 2024/12/06 12:53:08 by msennane         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

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

static int	is_a_word(char *str)
{
	if (!str)
		return (0);
	while (*str)
	{
		if (!ft_isalpha(*str))
			return (0);
		str++;
	}
	return (1);
}

static void	execute_builtin_command(t_exec *exec_cmd, t_shell_context *context,
		int *exit_status)
{
	int	status;

	if (ft_strcmp(exec_cmd->argv[0], "echo") == 0)
	{
		echo(exec_cmd->argv);
		*exit_status = 0;
	}
	else if (ft_strcmp(exec_cmd->argv[0], "exit") == 0)
	{
		ft_exit(exec_cmd->argv, context);
	}
	else if (ft_strcmp(exec_cmd->argv[0], "env") == 0)
		env(exec_cmd->argv, context);
	else if (ft_strcmp(exec_cmd->argv[0], "export") == 0)
		simple_export(exec_cmd->argv, context->env_vars);
	else if (ft_strcmp(exec_cmd->argv[0], "unset") == 0)
	{
		// free_exit(params, 0);
	}
	else if (ft_strcmp(exec_cmd->argv[0], "pwd") == 0)
	{
		pwd(exit_status);
	}
}
static void	execute_external_command(t_command *cmd, t_shell_context *context)
{
	t_exec	*exec_cmd;
	char	*binary_path;
	char	**envp;

	envp = envp_to_env_vector(context->env_vars);
	exec_cmd = (t_exec *)cmd;
	binary_path = get_command_path(exec_cmd->argv[0], context->env_vars);
	if (!binary_path)
	{
		print_exec_error(exec_cmd->argv[0], "command not found");
		terminate_cleanly(context, 127);
	}
	if (!file_exists(binary_path))
	{
		print_exec_error(exec_cmd->argv[0], "No such file or directory");
		terminate_cleanly(context, 127);
	}
	if (!is_executable(binary_path))
	{
		print_exec_error(exec_cmd->argv[0], "Permission denied");
		terminate_cleanly(context, 127);
	}
	handle_execve(binary_path, exec_cmd->argv, envp, context);
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
		terminate_cleanly(context, *exit_status);
	}
	else
	{
		if (exec_cmd->argv[0] && (access(exec_cmd->argv[0], X_OK) == 0))
		{
			execve(exec_cmd->argv[0], exec_cmd->argv, context->envp);
			print_exec_error(exec_cmd->argv[0], "command not found");
			terminate_cleanly(context, 127);
		}
		else
			execute_external_command(cmd, context);
	}
}
