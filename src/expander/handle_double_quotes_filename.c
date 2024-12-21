/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_double_quotes_filename.c                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msennane <msennane@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/21 16:05:46 by msennane          #+#    #+#             */
/*   Updated: 2024/12/21 16:09:22 by msennane         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	handle_double_quotes_filename(char *arg, int *values[2], t_queue_char *q,
		t_shell_context *context, int *was_quoted)
{
	int		*i;
	int		*exit_status;
	char	*exit_status_str;
	char	*var_name;
	char	*var_value;

	if (!arg || !values || !q || !context || !was_quoted)
		return ;
	i = values[0];
	exit_status = values[1];
	(*was_quoted) = 1;
	(*i)++;
	while (arg[*i] && arg[*i] != '\"')
	{
		if (arg[*i] == '$')
		{
			if (!arg[*i + 1] || is_whitespace(arg[*i + 1]) || arg[*i
				+ 1] == '\"')
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
			else
			{
				(*i)++;
				var_name = extract_variable_name(arg + *i);
				if (var_name)
				{
					var_value = get_env_value(var_name, context->env_vars);
					if (var_value)
						enqueue_str(q, var_value);
					(*i) += ft_strlen(var_name);
				}
			}
		}
		else
		{
			enqueue_char(q, arg[*i]);
			(*i)++;
		}
	}
	if (arg[*i] == '\"')
		(*i)++;
}
