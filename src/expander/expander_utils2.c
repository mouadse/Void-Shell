/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander_utils2.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msennane <msennane@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/07 23:39:28 by msennane          #+#    #+#             */
/*   Updated: 2024/12/20 23:23:19 by msennane         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

// static char	*replace_quotes_with_x(char *str)
// {
// 	char	*result;
// 	int		i;

// 	if (!str)
// 		return (NULL);
// 	result = ft_strdup(str);
// 	i = 0;
// 	while (result && result[i])
// 	{
// 		if (result[i] == '\'')
// 			result[i] = '\x1F';
// 		i++;
// 	}
// 	return (result);
// }
// int	has_special_characters(char *str)
// {
// 	if (!str)
// 		return (0);
// 	return (ft_strchr(str, '\'') || ft_strchr(str, '\"') || ft_strchr(str, '$')
// 			|| ft_strchr(str, '~') || ft_strchr(str, '\\'));
// }
// void	handle_single_quotes(char *str, int *index, t_queue_char *queue)
// {
// 	if (!str || !index || !queue)
// 		return ;
// 	enqueue_char(queue, '\'');
// 	(*index)++;
// 	while (str[*index] && str[*index] != '\'')
// 	{
// 		enqueue_char(queue, str[*index]);
// 		(*index)++;
// 	}
// 	if (str[*index] == '\'')
// 	{
// 		enqueue_char(queue, '\'');
// 		(*index)++;
// 	}
// }

// char	*extract_variable_name(char *arg)
// {
// 	int	i;

// 	if (!arg)
// 		return (NULL);
// 	if (arg[0] && (arg[0] == '@' || arg[0] == '*' || arg[0] == '#'
// 			|| arg[0] == '?' || arg[0] == '-' || arg[0] == '$' || arg[0] == '!'
// 			|| ft_isdigit(arg[0])))
// 	{
// 		return (ft_substr(arg, 0, 1));
// 	}
// 	if (!arg[0] || arg[0] == '=' || (!ft_isalpha(arg[0]) && arg[0] != '_'))
// 		return (ft_strdup(""));
// 	i = 0;
// 	while (arg[i] && (ft_isalnum(arg[i]) || arg[i] == '_'))
// 		i++;
// 	return (ft_substr(arg, 0, i));
// }
// void	process_variable(char *str, int *values[2], t_queue_char *queue,
// 		t_shell_context *context)
// {
// 	int		*i;
// 	char	*var_name;
// 	char	*var_value;

// 	if (!str || !values || !queue || !context)
// 		return ;
// 	i = values[0];
// 	var_name = extract_variable_name(str + *i);
// 	if (!var_name)
// 	{
// 		enqueue_char(queue, '$');
// 		return ;
// 	}
// 	var_value = get_env_value(var_name, context->env_vars);
// 	if (var_value)
// 		enqueue_str(queue, replace_quotes_with_x(var_value));
// 	else
// 		enqueue_char(queue, '\x01');
// 	(*i) += ft_strlen(var_name);
// }

// void	handle_dollar_sign(char *str, int *values[2], t_queue_char *queue,
// 		t_shell_context *context)
// {
// 	int		*index;
// 	int		*exit_status;
// 	char	*exit_status_str;

// 	if (!str || !values || !queue || !context)
// 		return ;
// 	index = values[0];
// 	exit_status = values[1];
// 	(*index)++;
// 	if (!str[*index] || is_whitespace(str[*index]))
// 	{
// 		enqueue_char(queue, '$');
// 		return ;
// 	}
// 	if (str[*index] == '?')
// 	{
// 		exit_status_str = ft_itoa(*exit_status);
// 		if (exit_status_str)
// 		{
// 			enqueue_str(queue, exit_status_str);
// 			(*index)++;
// 		}
// 	}
// 	else
// 		process_variable(str, values, queue, context);
// }

// void	handle_double_quotes(char *arg, int *values[2], t_queue_char *q,
// 		t_shell_context *context)
// {
// 	int		*i;
// 	int		*exit_status;
// 	char	*exit_status_str;
// 	char	*var_name;
// 	char	*var_value;

// 	if (!arg || !values || !q || !context)
// 		return ;
// 	i = values[0];
// 	exit_status = values[1];
// 	enqueue_char(q, '\"');
// 	(*i)++;
// 	while (arg[*i] && arg[*i] != '\"')
// 	{
// 		if (arg[*i] == '$')
// 		{
// 			if (!arg[*i + 1] || is_whitespace(arg[*i + 1]) || arg[*i
// 				+ 1] == '\"')
// 			{
// 				enqueue_char(q, '$');
// 				(*i)++;
// 			}
// 			else if (arg[*i + 1] == '?')
// 			{
// 				exit_status_str = ft_itoa(*exit_status);
// 				if (exit_status_str)
// 				{
// 					enqueue_str(q, exit_status_str);
// 					(*i) += 2;
// 				}
// 			}
// 			else if (arg[*i + 1] == '%')
// 			{
// 				enqueue_char(q, '$');
// 				(*i)++;
// 				while (arg[*i] == '%')
// 				{
// 					enqueue_char(q, '%');
// 					(*i)++;
// 				}
// 			}
// 			else
// 			{
// 				(*i)++;
// 				var_name = extract_variable_name(arg + *i);
// 				if (var_name)
// 				{
// 					var_value = get_env_value(var_name, context->env_vars);
// 					if (var_value)
// 						enqueue_str(q, replace_quotes_with_x(var_value));
// 					(*i) += ft_strlen(var_name);
// 				}
// 			}
// 		}
// 		else
// 		{
// 			enqueue_char(q, arg[*i]);
// 			(*i)++;
// 		}
// 	}
// 	if (arg[*i] == '\"')
// 	{
// 		enqueue_char(q, '\"');
// 		(*i)++;
// 	}
// }

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
