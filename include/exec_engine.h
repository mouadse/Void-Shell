/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_engine.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msennane <msennane@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/02 01:40:43 by msennane          #+#    #+#             */
/*   Updated: 2024/11/11 02:41:39 by msennane         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXEC_ENGINE_H
# define EXEC_ENGINE_H

# include "command.h"

# define SHELL_CHILD_PID_FILE "/tmp/shell_subprocess_pid.tmp"
# define SHELL_CHILD_STATUS_FILE "/tmp/shell_exit_status.tmp"
# define SHELL_HEREDOC_FILE "/tmp/shell_heredoc_content.tmp"

void	execute_command(t_command *cmd, t_shell_context *context,
			int *exit_status);
void	execute_pipeline_command(t_command *cmd, t_shell_context *context,
			int *exit_status);
void	clean_empty_arguments(t_exec *exec_cmd);
char	*get_executable_path(char *command, char *path);
void	handle_invalid_executable(t_exec *cmd, t_shell_context *context,
			struct stat path_stat);
char	*get_command_path(char *cmd_name, t_env_var *env_vars);
void	handle_execve(char *binary_path, char **argv, char **envp,
			t_shell_context *context);
void	print_exec_error(char *cmd_name, char *error_type);
void	handle_executable_path(t_exec *ecmd, t_shell_context *context);
#endif
