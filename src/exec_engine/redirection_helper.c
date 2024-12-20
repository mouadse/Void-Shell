/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection_helper.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msennane <msennane@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/22 14:06:13 by msennane          #+#    #+#             */
/*   Updated: 2024/12/20 22:56:08 by msennane         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static char	*getvar_name(char *arg)
{
	int	i;
	int	n;

	i = 0;
	if (ft_isdigit(arg[i]))
	{
		n = arg[i] - '0';
		return (ft_itoa(n));
	}
	while (arg[i] && ((ft_isalnum(arg[i]) || arg[i] == '_')))
		i++;
	return (ft_substr(arg, 0, i));
}

static void	replace_var_in_line(char *line, int *i, t_shell_context *context,
		t_queue_char *q)
{
	char	*var_name;
	char	*var_value;

	(*i)++;
	var_name = getvar_name(line + *i);
	var_value = get_env_value(var_name, context->env_vars);
	if (var_value)
		enqueue_str(q, var_value);
	*i += ft_strlen(var_name);
}

void	ignore_signals_heredoc(void)
{
	signal(SIGINT, SIG_IGN);
	signal(SIGQUIT, SIG_IGN);
}

char	*process_line_hd(t_shell_context *context, char *line,
		int *exit_status)
{
	int				i;
	t_queue_char	q;
	char			*exit_code;

	i = 0;
	init_queue_char(&q);
	while (line[i])
	{
		if (line[i] == '$' && line[i + 1] == '?')
		{
			exit_code = ft_itoa(*exit_status);
			enqueue_str(&q, exit_code);
			i += 2;
		}
		else if (line[i] == '$' && (is_whitespace(line[i + 1])))
			enqueue_char(&q, line[i++]);
		else if (line[i] == '$')
			replace_var_in_line(line, &i, context, &q);
		else
			enqueue_char(&q, line[i++]);
	}
	return (queue_char_str_convert(&q));
}
