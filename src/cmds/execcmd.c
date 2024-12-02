/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execcmd.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msennane <msennane@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/30 02:32:03 by msennane          #+#    #+#             */
/*   Updated: 2024/12/02 16:53:37 by msennane         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

t_command	*create_execcmd(void)
{
	t_exec	*cmd;

	cmd = ft_calloc(1, sizeof(*cmd));
	if (!cmd)
		return (NULL);    // Handle allocation failure
	cmd->type = CMD_EXEC; // Set the command type
	return ((t_command *)cmd);
}
