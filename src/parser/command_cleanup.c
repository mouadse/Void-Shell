/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_cleanup.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msennane <msennane@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/06 20:55:43 by msennane          #+#    #+#             */
/*   Updated: 2024/11/21 12:58:19 by msennane         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

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
		ft_free(exec);
	}
	else if (cmd->type == CMD_PIPE)
	{
		pipe = (t_pipe *)cmd;
		release_command_resources(pipe->left);
		release_command_resources(pipe->right);
		ft_free(pipe);
	}
	else if (cmd->type == CMD_REDIR)
	{
		redir = (t_redir *)cmd;
		release_command_resources(redir->sub_cmd);
		ft_free(redir);
	}
}
