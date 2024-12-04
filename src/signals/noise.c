/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   noise.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msennane <msennane@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/01 01:31:03 by msennane          #+#    #+#             */
/*   Updated: 2024/12/03 23:43:43 by msennane         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// This is the part responsible for the noise in the shell aka signal handling
// which involves 3 steps : Ignore signals, Handle signals, Default signals

#include "../../include/minishell.h"

void	signal_handler_general(int signum)
{
	if (signum == SIGINT)
	{
		ft_putstr_fd("\n", 1);
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
	}
}

void	signal_handler_input(int signum)
{
	char	*msg;

	if (signum == SIGQUIT)
	{
		msg = "Quit: 3\n"; // Assuming SIGQUIT is 3
		ft_putstr_fd(msg, 1);
	}
	else if (signum == SIGINT)
	{
		ft_putstr_fd("\n", 1);
	}
}

void	signal_handler_heredoc(int signum)
{
	int		fd;
	pid_t	child_pid;

	if (signum == SIGINT)
	{
		write(STDOUT_FILENO, "\n", 1);
		fd = open("/tmp/child_pid.tmp", O_RDONLY);
		if (fd < 0)
		{
			ft_putstr_fd("Error: Failed to open PID file\n", STDERR_FILENO);
			_exit(1);
		}
		if (read(fd, &child_pid, sizeof(pid_t)) < 0)
		{
			ft_putstr_fd("Error: Failed to read PID\n", STDERR_FILENO);
			close(fd);
			_exit(1);
		}
		close(fd);
		kill(child_pid, SIGKILL);
	}
	else if (signum == SIGQUIT)
		ft_putstr_fd("\b\b  \b\b", STDOUT_FILENO);
}

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
