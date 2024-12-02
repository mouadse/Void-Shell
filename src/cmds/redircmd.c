/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redircmd.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msennane <msennane@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/30 02:32:09 by msennane          #+#    #+#             */
/*   Updated: 2024/12/02 20:22:33 by msennane         ###   ########.fr       */
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

t_command	*create_redircmd(t_command *subcmd, t_redir_data redir,
		char redir_type)
{
	t_redir *cmd;

	cmd = ft_calloc(1, sizeof(*cmd));
	if (!cmd)
	{
		perror("ft_calloc failed");
		return (NULL);
	}
	cmd->type = CMD_REDIR;
	cmd->sub_cmd = subcmd;
	cmd->file = redir.file;
	cmd->efile = redir.efile;
	cmd->mode = redir.mode;
	cmd->fd = redir.fd;
	cmd->redir_type = redir_type;

	return ((t_command *)cmd); // Add this return statement
}
