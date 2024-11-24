/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   void-shell-utils1.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msennane <msennane@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/21 18:39:02 by msennane          #+#    #+#             */
/*   Updated: 2024/11/24 23:54:46 by msennane         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	retrieve_exit_status(t_command *cmd, t_shell_context *context,
		int *exit_status, int status)
{
	int	fd;

	if (cmd && (cmd->type == CMD_EXEC || cmd->type == CMD_REDIR))
	{
		if (WIFEXITED(status))
			*exit_status = WEXITSTATUS(status);
		else if (WIFSIGNALED(status))
			*exit_status = WTERMSIG(status) + 128;
		else
			*exit_status = EXIT_FAILURE;
	}
	else
	{
		fd = open(SHELL_CHILD_STATUS_FILE, O_RDONLY);
		if (-1 == fd)
			terminate_with_error(context, "open", 1);
		if (-1 == read(fd, exit_status, sizeof(int)))
			terminate_with_error(context, "read", 1);
		close(fd);
	}
}
