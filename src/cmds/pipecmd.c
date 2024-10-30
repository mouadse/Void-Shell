/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipecmd.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msennane <msennane@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/30 02:32:06 by msennane          #+#    #+#             */
/*   Updated: 2024/10/30 02:36:53 by msennane         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

t_command	*create_pipecmd(t_command *left, t_command *right)
{
	t_pipe	*pipecmd;

	pipecmd = malloc(sizeof(t_pipe));
	if (!pipecmd)
		return (NULL);
	pipecmd->type = CMD_PIPE;
	pipecmd->left = left;
	pipecmd->right = right;
	return ((t_command *)pipecmd);
}
