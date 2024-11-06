/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_cleanup.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msennane <msennane@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/06 20:55:43 by msennane          #+#    #+#             */
/*   Updated: 2024/11/06 20:59:08 by msennane         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static void	check_and_free(void *cmd)
{
	if (cmd)
	{
		free(cmd);
		cmd = NULL;
	}
}

void	release_command_resources(t_command *cmd)
{
	t_redir	*redir;
	t_pipe	*pipe;
	t_exec	*exec;

	if (!cmd)
		return ;
	if (cmd->type == CMD_EXEC)
	{
		exec = (t_exec *)cmd;
		check_and_free(exec);
	}
	else if (cmd->type == CMD_PIPE)
	{
		pipe = (t_pipe *)cmd;
		release_command_resources(pipe->left);
		release_command_resources(pipe->right);
		check_and_free(pipe);
	}
	else if (cmd->type == CMD_REDIR)
	{
		redir = (t_redir *)cmd;
		release_command_resources(redir->sub_cmd);
		check_and_free(redir);
	}
}
