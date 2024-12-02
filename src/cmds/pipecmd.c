/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipecmd.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msennane <msennane@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/30 02:32:06 by msennane          #+#    #+#             */
/*   Updated: 2024/12/02 16:48:29 by msennane         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

t_command	*create_pipecmd(t_command *left, t_command *right)
{
	t_pipe	*pipecmd;

	pipecmd = ft_calloc(1, sizeof(*pipecmd));
	if (!pipecmd)
		return (NULL);
	pipecmd->type = CMD_PIPE;
	pipecmd->left = left;
	pipecmd->right = right;
	return ((t_command *)pipecmd);
}
