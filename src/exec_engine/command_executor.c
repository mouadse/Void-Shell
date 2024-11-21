/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_executor.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msennane <msennane@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/01 23:46:19 by msennane          #+#    #+#             */
/*   Updated: 2024/11/21 16:36:54 by msennane         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	execute_command(t_command *cmd, t_shell_context *context,
		int *exit_status)
{
	if (CMD_EXEC == cmd->type)
	{
		// run our exec
	}
	else if (CMD_REDIR == cmd->type)
	{
		// run our redir
	}
	else if (CMD_PIPE == cmd->type)
	{
		execute_pipeline_command(cmd, context, exit_status);
	}
	exit(*exit_status);
}
