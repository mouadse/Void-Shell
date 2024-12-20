/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msennane <msennane@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/01 23:46:26 by msennane          #+#    #+#             */
/*   Updated: 2024/12/20 15:25:49 by msennane         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"
#include <stdio.h>

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
		if (full_path && (access(full_path, X_OK) == 0))
			return (full_path);
		i++;
	}
	return (NULL);
}

void	handle_invalid_executable(t_exec *cmd, t_shell_context *context,
		struct stat path_stat)
{
	if (S_ISDIR(path_stat.st_mode))
	{
		print_exec_error(cmd->argv[0], "Is a directory");
		terminate_cleanly(context, 126);
	}
	else if (access(cmd->argv[0], X_OK) != 0)
	{
		print_exec_error(cmd->argv[0], "Permission denied");
		terminate_cleanly(context, 126);
	}
}

void	handle_executable_path(t_exec *ecmd, t_shell_context *context)
{
	struct stat	path_stat;
	if (ecmd->argv[0] == NULL)
	{
		if (ft_strchr(context->input, '$') != NULL)
			terminate_cleanly(context, 0);
		else if (ft_strchr(context->input, '\"') || ft_strchr(context->input,
				'\'') && !ft_strchr(context->input, '<'))
			(print_exec_error(ecmd->argv[0], "command not found"),
				terminate_cleanly(context, 127));
		else
			terminate_cleanly(context, 0);
	}
	else if (ft_strchr("./", ecmd->argv[0][0]))
	{
		if (stat(ecmd->argv[0], &path_stat) == 0)
			handle_invalid_executable(ecmd, context, path_stat);
		else
			(print_exec_error(ecmd->argv[0], "No such file or directory"),
				terminate_cleanly(context, 127));
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
	print_exec_error(argv[0], "command not found");
	terminate_cleanly(context, 127);
}
