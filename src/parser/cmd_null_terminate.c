/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_null_terminate.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msennane <msennane@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/06 20:41:06 by msennane          #+#    #+#             */
/*   Updated: 2024/12/02 19:24:51 by msennane         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static void	nulterminate_pipe(t_command *cmd)
{
	t_pipe	*pipe;

	if (!cmd)
		return ;
	pipe = (t_pipe *)cmd;
	cmd_null_terminate(pipe->left);
	cmd_null_terminate(pipe->right);
}

static void	nulterminate_redir(t_command *cmd)
{
	t_redir	*redir;

	if (!cmd)
		return ;
	redir = (t_redir *)cmd;
	*redir->efile = '\0';
	cmd_null_terminate(redir->sub_cmd);
}

static void	nulterminate_exec(t_command *cmd)
{
	t_exec	*exec;
	int		i;

	if (!cmd)
		return ;
	exec = (t_exec *)cmd;
	i = 0;
	while (exec->argv[i] != NULL)
	{
		*exec->eargv[i] = '\0';
		i++;
	}
}

t_command	*cmd_null_terminate(t_command *cmd)
{
	if (!cmd)
		return (NULL);
	if (cmd->type == CMD_EXEC)
	{
		nulterminate_exec(cmd);
	}
	else if (cmd->type == CMD_PIPE)
	{
		nulterminate_pipe(cmd);
	}
	else if (cmd->type == CMD_REDIR)
	{
		nulterminate_redir(cmd);
	}
	return (cmd);
}
