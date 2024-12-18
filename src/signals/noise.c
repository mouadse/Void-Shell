/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   noise.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msennane <msennane@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/01 01:31:03 by msennane          #+#    #+#             */
/*   Updated: 2024/12/18 23:39:13 by msennane         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	setup_signals(void)
{
	setup_signal_action(SIGINT, signal_handler_general, SA_RESTART);
	setup_signal_action(SIGQUIT, SIG_IGN, 0);
}

void	set_signal_handler(t_command *tree)
{
	if (tree && tree->type == CMD_REDIR)
	{
		setup_signal_action(SIGINT, signal_handler_heredoc, SA_RESTART);
		setup_signal_action(SIGQUIT, signal_handler_heredoc, SA_RESTART);
	}
	else
	{
		setup_signal_action(SIGINT, signal_handler_input, SA_RESTART);
		setup_signal_action(SIGQUIT, signal_handler_input, SA_RESTART);
	}
}
