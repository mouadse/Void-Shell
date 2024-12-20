/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander_utils2.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msennane <msennane@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/07 23:39:28 by msennane          #+#    #+#             */
/*   Updated: 2024/12/20 00:39:29 by msennane         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static void	handle_dollar_sign_in_quotes(char *arg, int *i, int *exit_status,
		t_queue_char *q)
{
	char	*exit_status_str;

	if (!arg[*i + 1] || is_whitespace(arg[*i + 1]) || arg[*i + 1] == '\"')
	{
		enqueue_char(q, '$');
		(*i)++;
	}
	else if (arg[*i + 1] == '?')
	{
		exit_status_str = ft_itoa(*exit_status);
		if (exit_status_str)
		{
			enqueue_str(q, exit_status_str);
			(*i) += 2;
		}
	}
}

static void	handle_percent_in_quotes(char *arg, int *i, t_queue_char *q)
{
	enqueue_char(q, '$');
	(*i)++;
	while (arg[*i] == '%')
	{
		enqueue_char(q, '%');
		(*i)++;
	}
}

static void	handle_var_in_quotes(char *arg, int *i, t_queue_char *q,
		t_shell_context *context)
{
	char	*var_name;
	char	*var_value;

	(*i)++;
	var_name = extract_variable_name(arg + *i);
	if (var_name)
	{
		var_value = get_env_value(var_name, context->env_vars);
		if (var_value)
			enqueue_str(q, replace_quotes_with_x(var_value));
		(*i) += ft_strlen(var_name);
	}
}

static void	process_quotes_content(char *arg, int *values[2], t_queue_char *q,
		t_shell_context *context)
{
	while (arg[*values[0]] && arg[*values[0]] != '\"')
	{
		if (arg[*values[0]] == '$')
		{
			if (arg[*values[0] + 1] == '%')
				handle_percent_in_quotes(arg, values[0], q);
			else if (!arg[*values[0] + 1] || is_whitespace(arg[*values[0] + 1])
				|| arg[*values[0] + 1] == '\"' || arg[*values[0] + 1] == '?')
				handle_dollar_sign_in_quotes(arg, values[0], values[1], q);
			else
				handle_var_in_quotes(arg, values[0], q, context);
		}
		else
		{
			enqueue_char(q, arg[*values[0]]);
			(*values[0])++;
		}
	}
}

void	handle_double_quotes(char *arg, int *values[2], t_queue_char *q,
		t_shell_context *context)
{
	int	*i;
	int	*exit_status;

	if (!arg || !values || !q || !context)
		return ;
	i = values[0];
	exit_status = values[1];
	enqueue_char(q, '\"');
	(*i)++;
	process_quotes_content(arg, values, q, context);
	if (arg[*i] == '\"')
	{
		enqueue_char(q, '\"');
		(*i)++;
	}
}
