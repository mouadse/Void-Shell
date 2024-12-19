/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander_filename.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msennane <msennane@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/07 23:39:15 by msennane          #+#    #+#             */
/*   Updated: 2024/12/20 00:27:11 by msennane         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static void	handle_single_quotes2(char *str, int *index, t_queue_char *queue,
		int *was_quoted)
{
	if (!str || !index || !queue || !was_quoted)
		return ;
	(*was_quoted) = 1;
	(*index)++;
	while (str[*index] && str[*index] != '\'')
	{
		enqueue_char(queue, str[*index]);
		(*index)++;
	}
	if (str[*index] == '\'')
	{
		(*index)++;
	}
}

static void	handle_dollar_in_quotes2(t_quote_params *params)
{
	char	*exit_status_str;
	char	*var_name;
	char	*var_value;

	if (!params->arg[*params->i + 1] || is_whitespace(params->arg[*params->i + 1])
		|| params->arg[*params->i + 1] == '\"')
	{
		enqueue_char(params->q, '$');
		(*params->i)++;
		return ;
	}
	if (params->arg[*params->i + 1] == '?')
	{
		exit_status_str = ft_itoa(*params->exit_status);
		if (exit_status_str)
		{
			enqueue_str(params->q, exit_status_str);
			(*params->i) += 2;
		}
		return ;
	}
	(*params->i)++;
	var_name = extract_variable_name(params->arg + *params->i);
	if (var_name && (var_value = get_env_value(var_name, params->context->env_vars)))
		(enqueue_str(params->q, var_value), (*params->i) += ft_strlen(var_name));
}

static void	handle_double_quotes2(t_quote_params *params)
{
	if (!params->arg || !params->q || !params->context || !params->was_quoted)
		return ;
	(*params->was_quoted) = 1;
	(*params->i)++;
	while (params->arg[*params->i] && params->arg[*params->i] != '\"')
	{
		if (params->arg[*params->i] == '$')
			handle_dollar_in_quotes2(params);
		else
		{
			enqueue_char(params->q, params->arg[*params->i]);
			(*params->i)++;
		}
	}
	if (params->arg[*params->i] == '\"')
		(*params->i)++;
}


static void	process_argument2(char *arg, t_queue_char *queue, int *exit_status,
		t_shell_context *context, int *was_quoted)
{
	int	i;
	int	*values[2];
	int	prev_i;
	t_quote_params params;

	if (!arg || !queue || !exit_status || !context || !was_quoted)
		return ;
	i = 0;
	while (arg[i] != '\0')
	{
		prev_i = i;
		values[0] = &i;
		values[1] = exit_status;
		if (arg[i] == '\'')
			handle_single_quotes2(arg, &i, queue, was_quoted);
		else if (arg[i] == '\"')
		{
			params.arg = arg;
			params.i = values[0];
			params.q = queue;
			params.context = context;
			params.was_quoted = was_quoted;
			handle_double_quotes2(&params);
		}
		else if (arg[i] == '$')
			handle_dollar_sign(arg, values, queue, context);
		else
			enqueue_char(queue, arg[i++]);
		if (prev_i == i && arg[i] != '\0')
			i++;
	}
}

char	*clean_argument2(char *arg, t_shell_context *context, int *exit_status)
{
	t_queue_char	queue;
	char			*cleaned_arg;
	char			*home;
	int				was_quoted;

	was_quoted = 0;
	init_queue_char(&queue);
	if (ft_strcmp(arg, "~") == 0)
	{
		home = get_env_value("HOME", context->env_vars);
		if (home)
			cleaned_arg = home;
		else
			cleaned_arg = ft_strdup("~");
	}
	else
	{
		process_argument2(arg, &queue, exit_status, context, &was_quoted);
		cleaned_arg = queue_char_str_convert(&queue);
	}
	if (is_ambiguous_redirect(cleaned_arg) && !was_quoted)
	{
		ft_putstr_fd("minishell: ambiguous redirect\n", STDERR_FILENO);
		cleaned_arg = ft_strdup("\x01\x01\x01");
	}
	return (cleaned_arg);
}
