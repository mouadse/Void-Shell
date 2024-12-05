/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution_manager.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msennane <msennane@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/01 23:46:23 by msennane          #+#    #+#             */
/*   Updated: 2024/12/05 12:21:09 by msennane         ###   ########.fr       */
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
	int		status;
	char	cwd[1024];

	if (ft_strcmp(exec_cmd->argv[0], "echo") == 0)
	{
		echo(exec_cmd->argv);
		*exit_status = 0;
	}
	else if (ft_strcmp(exec_cmd->argv[0], "exit") == 0)
	{
		// Implement exit functionality
		status = 0;
		if (exec_cmd->argv[1] && is_a_word(exec_cmd->argv[1]))
			status = 2;
		else if (exec_cmd->argv[2] != NULL)
			status = 1;
		else if (exec_cmd->argv[1])
			status = atoi(exec_cmd->argv[1]);
		terminate_cleanly(context, status);
	}
	else if (ft_strcmp(exec_cmd->argv[0], "env") == 0)
	{
		// Implement env functionality
		for (int i = 0; context->envp[i]; i++)
			printf("%s\n", context->envp[i]);
		*exit_status = 0;
	}
	else if (ft_strcmp(exec_cmd->argv[0], "export") == 0)
	{
		simple_export(exec_cmd->argv, context->env_vars);
		*exit_status = 0;
	}
	else if (ft_strcmp(exec_cmd->argv[0], "unset") == 0)
	{
		*exit_status = 0;
	}
	else if (ft_strcmp(exec_cmd->argv[0], "pwd") == 0)
	{
		// gc_free_all();
		// Implement pwd functionality
		if (getcwd(cwd, sizeof(cwd)) != NULL)
		{
			printf("%s\n", cwd);
			*exit_status = 0;
		}
		else
		{
			print_exec_error(exec_cmd->argv[0],
				"Unable to get current directory");
			*exit_status = 1;
		}
	}
	else
	{
		print_exec_error(exec_cmd->argv[0], "Unknown builtin command");
		*exit_status = 1;
	}
	// gc_free_all();
}

// sh lvl here

char	*ft_strjoin3(const char *s1, const char *s2, const char *s3)
{
	char	*result;
	size_t	len1;
	size_t	len2;
	size_t	len3;

	if (!s1 || !s2 || !s3)
		return (NULL);
	len1 = ft_strlen(s1);
	len2 = ft_strlen(s2);
	len3 = ft_strlen(s3);
	result = (char *)gc_malloc(len1 + len2 + len3 + 1);
	if (!result)
		return (NULL);
	ft_memcpy(result, s1, len1);
	ft_memcpy(result + len1, s2, len2);
	ft_memcpy(result + len1 + len2, s3, len3);
	result[len1 + len2 + len3] = '\0';
	return (result);
}

char	**envp_to_env_vector(t_env_var *env_vars)
{
	int			i;
	int			count;
	t_env_var	*temp;
	char		**env_vector;

	i = 0;
	count = 0;
	temp = env_vars;
	// Count the number of environment variables
	while (temp)
	{
		count++;
		temp = temp->next;
	}
	env_vector = gc_malloc(sizeof(char *) * (count + 1));
	if (!env_vector)
		return (NULL);
	temp = env_vars;
	while (temp)
	{
		if (temp->key)
		{
			if (temp->value)
				env_vector[i] = ft_strjoin3(temp->key, "=", temp->value);
			else
				env_vector[i] = ft_strdup(temp->key);
			if (!env_vector[i])
				return (NULL);
			i++;
		}
		temp = temp->next;
	}
	env_vector[i] = NULL;
	return (env_vector);
}

static void	execute_external_command(t_command *cmd, t_shell_context *context)
{
	t_exec	*exec_cmd;
	char	*binary_path;
	char	**envp;

	envp = envp_to_env_vector(context->env_vars);
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
		// ft_free(binary_path);
		terminate_cleanly(context, 127);
	}
	if (!is_executable(binary_path))
	{
		print_exec_error(exec_cmd->argv[0], "Permission denied");
		// ft_free(binary_path);
		terminate_cleanly(context, 127);
	}
	/* Attempt to execute the command */
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
		// Do not terminate here; allow the shell to continue running
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
		{
			execute_external_command(cmd, context);
		}
	}
}
