/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   noise_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msennane <msennane@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/01 01:31:03 by msennane          #+#    #+#             */
/*   Updated: 2024/12/18 23:39:03 by msennane         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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
		msg = "Quit: 3\n";
		ft_putstr_fd(msg, 1);
	}
	else if (signum == SIGINT)
		ft_putstr_fd("\n", 1);
}

void	signal_handler_heredoc(int signum)
{
	int		fd;
	pid_t	child_pid;

	if (signum == SIGINT)
	{
		write(STDOUT_FILENO, "\n", 1);
		fd = open(SHELL_CHILD_PID_FILE, O_RDONLY);
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
