/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_executor.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msennane <msennane@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/01 23:46:19 by msennane          #+#    #+#             */
/*   Updated: 2024/12/02 19:05:36 by msennane         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"
#include <stdio.h>
#include <unistd.h>

void	execute_command(t_command *cmd, t_shell_context *context,
		int *exit_status)
{
	printf("DEBUG: Executing command with type=%d\n", cmd->type);
	printf("DEBUG: CMD_EXEC=%d, CMD_REDIR=%d, CMD_PIPE=%d\n", CMD_EXEC,
		CMD_REDIR, CMD_PIPE);

	if (!cmd)
	{
		printf("Error: NULL command\n");
		*exit_status = 1;
		return ;
	}

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
	else
	{
		printf("Error: Unknown command type %d\n", cmd->type);
		*exit_status = 1;
	}

	exit(*exit_status);
}
