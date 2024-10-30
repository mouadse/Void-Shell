/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redircmd.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msennane <msennane@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/30 02:32:09 by msennane          #+#    #+#             */
/*   Updated: 2024/10/30 02:38:27 by msennane         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

t_redir_data	create_redirdata(char *file, char *efile, int fd, int mode)
{
	t_redir_data	data;

	data.file = file;
	data.efile = efile;
	data.fd = fd;
	data.mode = mode;
	return (data);
}

t_command	*create_redircmd(t_command *sub_cmd, t_redir_data data,
		char redir_type)
{
	t_redir	*redir_cmd;

	redir_cmd = malloc(sizeof(t_redir));
	if (!redir_cmd)
		return (NULL);
	redir_cmd->sub_cmd = sub_cmd;
	redir_cmd->type = CMD_REDIR;
	redir_cmd->redir_type = redir_type;
	redir_cmd->file = data.file;
	redir_cmd->efile = data.efile;
	redir_cmd->fd = data.fd;
	redir_cmd->mode = data.mode;
	return ((t_command *)redir_cmd);
}
