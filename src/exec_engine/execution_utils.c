/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msennane <msennane@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/01 23:46:26 by msennane          #+#    #+#             */
/*   Updated: 2024/11/21 16:09:43 by msennane         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	clean_empty_arguments(t_exec *exec_cmd)
{
	int	i;

	i = 0;
	while (exec_cmd->argv[0] && exec_cmd->argv[0][0] != '\0')
	{
		i = 0;
		while (exec_cmd->argv[i] && exec_cmd->argv[i + 1])
		{
			exec_cmd->argv[i] = exec_cmd->argv[i + 1];
			i++;
		}
		exec_cmd->argv[i] = NULL;
	}
}

char	*get_executable_path(char *command, char *path)
{
	int		i;
	char	**path_directories;
	char	*command_with_slash;
	char	*full_path;

	path_directories = ft_split(path, ':');
	command_with_slash = ft_strjoin("/", command);
	i = 0;
	while (path_directories[i])
	{
		full_path = ft_strjoin(path_directories[i], command_with_slash);
		if (access(full_path, X_OK) == 0)
		{
			// free(command);
			// free_split(path_directories);
			return (full_path);
		}
		free(full_path);
		i++;
	}
	// free(command);
	// free_split(path_directories);
	return (NULL);
}

void	handle_invalid_executable(t_exec *cmd, t_shell_context *context,
		struct stat path_stat)
{
	if (S_ISDIR(path_stat.st_mode))
	{
		// ft_printf_fd(STDERR_FILENO, "minishell: %s: is a directory\n",
		// cmd->argv[0]);
		// ft_putstr_fd("Void-shell: ", STDERR_FILENO);
		// ft_putstr_fd(cmd->argv[0], STDERR_FILENO);
		// ft_putstr_fd(": is a directory\n", STDERR_FILENO);
		print_exec_error(cmd->argv[0], "is a directory");
		terminate_cleanly(context, 127);
	}
	else if (access(cmd->argv[0], X_OK) != 0)
	{
		// ft_printf_fd(STDERR_FILENO, "minishell: %s: Permission denied\n",
		//              cmd->argv[0]);
		// ft_putstr_fd("Void-shell: ", STDERR_FILENO);
		// ft_putstr_fd(cmd->argv[0], STDERR_FILENO);
		// ft_putstr_fd(": Permission denied\n", STDERR_FILENO);
		print_exec_error(cmd->argv[0], "Permission denied");
		terminate_cleanly(context, 127);
	}
}

void	handle_executable_path(t_exec *ecmd, t_shell_context *context)
{
	struct stat	path_stat;

	if (ecmd->argv[0] == NULL)
		terminate_cleanly(context, 127);
	else if (ft_strchr("./", ecmd->argv[0][0]))
	{
		if (stat(ecmd->argv[0], &path_stat) == 0)
			handle_invalid_executable(ecmd, context, path_stat);
		else
		{
			// ft_printf_fd(STDERR_FILENO,
			// 	"minishell: %s: No such file or directory\n",
			// 	ecmd->argv[0]);
			print_exec_error(ecmd->argv[0], "No such file or directory");
			terminate_cleanly(context, 127);
		}
	}
}
char	*get_command_path(char *cmd_name, t_env_var *env_vars)
{
	char	*path_env;
	char	*cmd_path;

	path_env = get_env_value("PATH", env_vars);
	if (!path_env)
		return (NULL);
	cmd_path = get_executable_path(cmd_name, path_env);
	return (cmd_path);
}

void	handle_execve(char *binary_path, char **argv, char **envp,
		t_shell_context *context)
{
	execve(binary_path, argv, envp);
	print_exec_error(argv[0], "Error executing command");
	ft_free(binary_path); // replace by custom free function
	terminate_cleanly(context, 127);
}
