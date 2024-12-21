/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_double_quotes_filename.c                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msennane <msennane@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/21 16:05:46 by msennane          #+#    #+#             */
/*   Updated: 2024/12/21 16:21:10 by msennane         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

// void	handle_double_quotes_filename(char *arg, int *values[2], t_queue_char *q,
// 		t_shell_context *context, int *was_quoted)
// {
// 	int		*i;
// 	int		*exit_status;
// 	char	*exit_status_str;
// 	char	*var_name;
// 	char	*var_value;

// 	if (!arg || !values || !q || !context || !was_quoted)
// 		return ;
// 	i = values[0];
// 	exit_status = values[1];
// 	(*was_quoted) = 1;
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
// 			else
// 			{
// 				(*i)++;
// 				var_name = extract_variable_name(arg + *i);
// 				if (var_name)
// 				{
// 					var_value = get_env_value(var_name, context->env_vars);
// 					if (var_value)
// 						enqueue_str(q, var_value);
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
// 		(*i)++;
// }

static void	handle_dollar_question(t_queue_char *q, int *exit_status, int *i)
{
	char	*exit_status_str;

	exit_status_str = ft_itoa(*exit_status);
	if (exit_status_str)
	{
		enqueue_str(q, exit_status_str);
	}
	(*i) += 2;
}

static void	handle_variable_expansion(char *arg, int *i, t_queue_char *q, t_shell_context *context)
{
	char	*var_name;
	char	*var_value;
	size_t	var_len;

	var_name = extract_variable_name(arg + *i);
	if (var_name)
	{
		var_len = ft_strlen(var_name);
		var_value = get_env_value(var_name, context->env_vars);
		if (var_value)
			enqueue_str(q, var_value);
		(*i) += var_len;
	}
}


static void	handle_dollar_sign_file(char *arg, int *i, t_queue_char *q,
								int *exit_status, t_shell_context *context)
{
	if (!arg[*i + 1] || is_whitespace(arg[*i + 1]) || arg[*i + 1] == '\"')
	{
		enqueue_char(q, '$');
		(*i)++;
	}
	else if (arg[*i + 1] == '?')
	{
		handle_dollar_question(q, exit_status, i);
	}
	else
	{
		(*i)++;
		handle_variable_expansion(arg, i, q, context);
	}
}

static void	process_double_quoted_content(char *arg, int *i, t_queue_char *q,
										  int *exit_status, t_shell_context *context)
{
	while (arg[*i] && arg[*i] != '\"')
	{
		if (arg[*i] == '$')
			handle_dollar_sign_file(arg, i, q, exit_status, context);
		else
		{
			enqueue_char(q, arg[*i]);
			(*i)++;
		}
	}
}

void	handle_double_quotes_filename(char *arg, int *values[2], t_queue_char *q,
									  t_shell_context *context)
{
	int		*i;
	int		*exit_status;

	if (!arg || !values || !q || !context)
		return ;

	i = values[0];
	exit_status = values[1];

	(*i)++;
	process_double_quoted_content(arg, i, q, exit_status, context);

	if (arg[*i] == '\"')
		(*i)++;
}
