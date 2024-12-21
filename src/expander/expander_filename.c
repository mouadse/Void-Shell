/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander_filename.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msennane <msennane@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/07 23:39:15 by msennane          #+#    #+#             */
/*   Updated: 2024/12/21 16:23:13 by msennane         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static void	handle_single_quotes2(char *str, int *index, t_queue_char *queue)
{
	if (!str || !index || !queue)
		return ;
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

static void	process_argument2(char *arg, t_queue_char *queue, int *exit_status,
		t_shell_context *context)
{
	int	i;
	int	prev_i;
	int	*values[2];

	if (!arg || !queue || !exit_status || !context)
		return ;
	i = 0;
	values[0] = &i;
	values[1] = exit_status;
	while (arg[i])
	{
		prev_i = i;
		if (arg[i] == '\'')
			handle_single_quotes2(arg, &i, queue);
		else if (arg[i] == '\"')
			handle_double_quotes_filename(arg, values, queue, context);
		else if (arg[i] == '$')
			handle_dollar_sign(arg, values, queue, context);
		else
			enqueue_char(queue, arg[i++]);
		if (prev_i == i && arg[i])
			i++;
	}
}

char	*clean_argument2(char *arg, t_shell_context *context, int *exit_status)
{
	t_queue_char	queue;
	char			*cleaned_arg;
	char			*home;

	// int				was_quoted;
	// was_quoted = 0;
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
		process_argument2(arg, &queue, exit_status, context);
		cleaned_arg = queue_char_str_convert(&queue);
	}
	if (is_ambiguous_redirect(cleaned_arg) && (!ft_strchr(context->input, '\"')
			&& !ft_strchr(context->input, '\''))) // quotes are ambiguous
	{
		ft_putstr_fd("minishell: ambiguous redirect\n", STDERR_FILENO);
		cleaned_arg = ft_strdup("\x01\x01\x01");
	}
	return (cleaned_arg);
}
