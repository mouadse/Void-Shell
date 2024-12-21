/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process_argument.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msennane <msennane@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/21 01:09:05 by msennane          #+#    #+#             */
/*   Updated: 2024/12/21 01:39:07 by msennane         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

// void	process_argument(char *arg, t_queue_char *queue, int *exit_status,
// 		t_shell_context *context)
// {
// 	int	i;
// 	int	*values[2];
// 	int	prev_i;

// 	if (!arg || !queue || !exit_status || !context)
// 		return ;
// 	i = 0;
// 	while (arg[i] != '\0')
// 	{
// 		prev_i = i;
// 		if (arg[i] == '\'')
// 			handle_single_quotes(arg, &i, queue);
// 		else if (arg[i] == '\"')
// 		{
// 			values[0] = &i;
// 			values[1] = exit_status;
// 			handle_double_quotes(arg, values, queue, context);
// 		}
// 		else if (arg[i + 1] == '%')
// 		{
// 			enqueue_char(queue, '$');
// 			i++;
// 			while (arg[i] == '%')
// 			{
// 				enqueue_char(queue, '%');
// 				i++;
// 			}
// 		}
// 		else if (arg[i] == '$')
// 		{
// 			values[0] = &i;
// 			values[1] = exit_status;
// 			handle_dollar_sign(arg, values, queue, context);
// 		}
// 		else
// 			enqueue_char(queue, arg[i++]);
// 		if (prev_i == i && arg[i] != '\0')
// 			i++;
// 	}
// }

void	enqueue_percentage_signs(const char *arg, int *i, t_queue_char *queue)
{
	enqueue_char(queue, '$');
	(*i)++;
	while (arg[*i] && arg[*i] == '%')
    {
        enqueue_char(queue, '%');
        (*i)++;
    }
}

void	process_special_character(char *arg, int *i, t_queue_char *queue,
		t_shell_context *context)
{
	int	*values[2];

	if (arg[*i] == '\'')
		handle_single_quotes(arg, i, queue);
	else if (arg[*i] == '\"')
	{
		values[0] = i;
		values[1] = context->exit_status;
		handle_double_quotes(arg, values, queue, context);
	}
	else if (arg[*i] && arg[*i + 1] && arg[*i + 1] == '%')
		enqueue_percentage_signs(arg, i, queue);
	else if (arg[*i] == '$')
	{
		values[0] = i;
		values[1] = context->exit_status;
		handle_dollar_sign(arg, values, queue, context);
	}
}

void	process_argument(char *arg, t_queue_char *queue, int *exit_status,
		t_shell_context *context)
{
	int	i;
	int	prev_i;

	if (!arg || !queue || !exit_status || !context)
		return ;
	i = 0;
	context->exit_status = exit_status;
	while (arg[i] != '\0')
	{
		prev_i = i;
		if (arg[i] == '\'' || arg[i] == '\"' || arg[i + 1] == '%'
			|| arg[i] == '$')
			process_special_character(arg, &i, queue, context);
		else
			enqueue_char(queue, arg[i++]);
		if (prev_i == i && arg[i] != '\0')
			i++;
	}
}
