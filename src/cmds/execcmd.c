/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execcmd.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msennane <msennane@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/30 02:32:03 by msennane          #+#    #+#             */
/*   Updated: 2024/10/30 02:33:41 by msennane         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

t_command	*create_execcmd(void)
{
	t_exec	*exec_cmd;

	exec_cmd = malloc(sizeof(t_exec));
	if (!exec_cmd)
		return (NULL);
	memset(exec_cmd, 0, sizeof(t_exec)); // to be changed later with my own func
	exec_cmd->type = CMD_EXEC;
	return ((t_command *)exec_cmd);
}
