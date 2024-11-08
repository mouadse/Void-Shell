/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   validator.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msennane <msennane@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/07 23:38:33 by msennane          #+#    #+#             */
/*   Updated: 2024/11/07 23:38:57 by msennane         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static void	display_error(int ret)
{
	ft_putstr_fd("Void-shell: : unexpected EOF while looking for matching `",
		STDERR_FILENO);
	ft_putchar_fd(ret, STDERR_FILENO);
	ft_putstr_fd("'\n", STDERR_FILENO);
}

static int	check_unbalanced_quotes(char *line, int *exit_status)
{
	int	ret;
	int	i;

	i = 0;
	while (line[i] != '\0')
	{
		if (line[i] == '\"' || line[i] == '\'')
		{
			ret = line[i];
			i++;
			while (line[i] && line[i] != ret)
				i++;
			if (line[i] == '\0')
			{
				display_error(ret);
				*exit_status = 258;
				return (0);
			}
			i++;
		}
		else
			i++;
	}
	return (1);
}

static int	check_command_syntax(char *line, int *exit_status)
{
	char	*ps;
	char	*es;
	int		ret;

	ps = line;
	es = ps + ft_strlen(ps);
	if (!validate_pipe_syntax(&ps, es, exit_status))
		return (0);
	while (peek(&ps, es, "\0") == 0)
	{
		ret = gettoken(&ps, es, 0, 0);
		if (ret == '>' || ret == '<' || ret == '+' || ret == '%')
		{
			if (!check_redirection_syntax(&ps, es, exit_status))
				return (0);
		}
		else if (ret == '|')
		{
			if (!check_pipe_usage(&ps, es, exit_status))
				return (0);
		}
		else if (ret == '\0')
			break ;
	}
	return (1);
}

int	check_command_validity(char *line, int *exit_status)
{
	if (!check_unbalanced_quotes(line, exit_status))
		return (0);
	if (!check_command_syntax(line, exit_status))
		return (0);
	return (1);
}
