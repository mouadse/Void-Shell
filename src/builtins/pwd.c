/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msennane <msennane@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/05 16:03:46 by msennane          #+#    #+#             */
/*   Updated: 2024/12/05 16:38:44 by msennane         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	pwd(int *exit_status)
{
	char	*cwd;

	cwd = getcwd(NULL, 0);
	if (!cwd)
	{
		*exit_status = 1;
		perror("pwd");
		return ;
	}
	ft_putendl_fd(cwd, STDOUT_FILENO);
	free(cwd);
	*exit_status = 0;
}
