/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_executor.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msennane <msennane@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/01 23:46:19 by msennane          #+#    #+#             */
/*   Updated: 2024/12/02 20:22:18 by msennane         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	execute_command(t_command *cmd, t_shell_context *context,
		int *exit_status)
{
	if (cmd->type == CMD_EXEC)
	{
		run_exec(cmd, context, exit_status);
	}
	else if (cmd->type == CMD_REDIR)
	{
		execute_redirects_command(cmd, context, exit_status);
	}
	else if (cmd->type == CMD_PIPE)
	{
		execute_pipeline_command(cmd, context, exit_status);
	}
	exit(*exit_status);
}
