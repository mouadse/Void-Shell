/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_engine.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msennane <msennane@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/02 01:40:43 by msennane          #+#    #+#             */
/*   Updated: 2024/11/03 18:19:20 by msennane         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXEC_ENGINE_H
# define EXEC_ENGINE_H

#include "command.h"

# define SHELL_CHILD_PID_FILE "/tmp/shell_subprocess_pid.tmp"
# define SHELL_CHILD_STATUS_FILE "/tmp/shell_exit_status.tmp"
# define SHELL_HEREDOC_FILE "/tmp/shell_heredoc_content.tmp"

void	execute_command(t_command *cmd, t_shell_context *context,
		int *exit_status);
void	execute_pipeline_command(t_command *cmd, t_shell_context *context,
		int *exit_status);


#endif
