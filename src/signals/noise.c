/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   noise.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msennane <msennane@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/01 01:31:03 by msennane          #+#    #+#             */
/*   Updated: 2024/12/18 23:40:22 by msennane         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static void	setup_signal_action(int signum, void (*handler)(int), int flags)
{
	struct sigaction	act;

	sigemptyset(&act.sa_mask);
	act.sa_handler = handler;
	act.sa_flags = flags;
	sigaction(signum, &act, NULL);
}

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
