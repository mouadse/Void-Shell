/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   void-shell-utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msennane <msennane@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/21 18:39:02 by msennane          #+#    #+#             */
/*   Updated: 2024/11/24 23:34:52 by msennane         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	is_whitespace(char c)
{
	if (c == ' ' || c == '\t' || c == '\n' || c == '\v' || c == '\f'
		|| c == '\r')
		return (1);
	return (0);
}

int	is_whitespace_string(char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (!is_whitespace(str[i]))
			return (0);
		i++;
	}
	return (1);
}

int	is_numeric(const char *str)
{
	if (*str && (*str == '+' || *str == '-'))
		str++;
	if (*str == '\0')
		return (0);
	while (*str)
	{
		if (*str < '0' || *str > '9')
			return (0);
		str++;
	}
	return (1);
}

int	ft_fork(t_shell_context *context)
{
	int	pid;

	pid = fork();
	if (pid == -1)
		terminate_with_error(context, "fork", 1);
	return (pid);
}

void	ft_pipe(int fd[2], t_shell_context *context)
{
	if (pipe(fd) == -1)
		terminate_with_error(context, "fork", 1);
}
