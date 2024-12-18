/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_redir.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msennane <msennane@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/22 14:06:13 by msennane          #+#    #+#             */
/*   Updated: 2024/12/19 00:22:03 by msennane         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

// static void	handle_dollar_sign_del(char *str, int *index,
// t_queue_char *queue)
// {
// 	int	i;

// 	i = *index;
// 	if (str[i + 1] == '"')
// 	{
// 		i += 2;
// 		while (str[i] && str[i] != '"')
// 		{
// 			enqueue_char(queue, str[i]);
// 			i++;
// 		}
// 		if (str[i] == '"')
// 			i++;
// 	}
// 	else if (str[i + 1] && (str[i + 1] == '$' || str[i + 1] == '\0' || str[i
// 			+ 1] == ' '))
// 	{
// 		enqueue_char(queue, str[i]);
// 		i++;
// 	}
// 	else
// 	{
// 		enqueue_char(queue, str[i]);
// 		i++;
// 		if (str[i])
// 		{
// 			enqueue_char(queue, str[i]);
// 			i++;
// 		}
// 	}
// 	*index = i - 1;
// }

static void	handle_dollar_sign_del(char *str, int *index, t_queue_char *queue)
{
	int	i;

	i = *index;
	if (str[i + 1] == '"')
	{
		i += 2;
		while (str[i] && str[i] != '"')
			enqueue_char(queue, str[i++]);
		if (str[i] == '"')
			i++;
	}
	else if (str[i + 1] && (str[i + 1] == '$' || str[i + 1] == '\0' || str[i
			+ 1] == ' '))
		enqueue_char(queue, str[i++]);
	else
	{
		enqueue_char(queue, str[i++]);
		if (str[i])
			enqueue_char(queue, str[i++]);
	}
	*index = i - 1;
}

static void	handle_single_quote(char *str, int *index, t_queue_char *queue)
{
	int	i;

	(*index)++;
	i = *index;
	while (str[i] && str[i] != '\'')
	{
		enqueue_char(queue, str[i]);
		i++;
	}
	*index = i;
}

static void	handle_double_quote(char *str, int *index, t_queue_char *queue)
{
	int	i;

	(*index)++;
	i = *index;
	while (str[i] && str[i] != '"')
	{
		enqueue_char(queue, str[i]);
		i++;
	}
	*index = i;
}

static void	clean_del_helper(char *str, t_queue_char *queue, int *is_quoted)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (str[i] == '\'')
		{
			handle_single_quote(str, &i, queue);
			*is_quoted = 1;
		}
		else if (str[i] == '"')
		{
			handle_double_quote(str, &i, queue);
			*is_quoted = 1;
		}
		else if (str[i] == '$')
		{
			handle_dollar_sign_del(str, &i, queue);
			*is_quoted = 1;
		}
		else
			enqueue_char(queue, str[i]);
		i++;
	}
}

char	*clean_delimiter(char *str, int *is_quoted)
{
	t_queue_char	queue;
	char			*cleaned;

	init_queue_char(&queue);
	*is_quoted = 0;
	clean_del_helper(str, &queue, is_quoted);
	cleaned = queue_char_str_convert(&queue);
	return (cleaned);
}
