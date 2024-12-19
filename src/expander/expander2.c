/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander2.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msennane <msennane@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/07 23:39:15 by msennane          #+#    #+#             */
/*   Updated: 2024/12/20 00:01:31 by msennane         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static void	handle_double_quotes_rem(const char *str, int *i,
		int *in_double_quotes)
{
	if (*in_double_quotes == 0)
		*in_double_quotes = 1;
	else
		*in_double_quotes = 0;
	(*i)++;
}

static void	handle_consecutive_quotes(const char *str, char *result, int *i,
		int *j)
{
	int	count;
	int	k;

	count = 0;
	k = *i;
	while (str[k] == '\'')
	{
		count++;
		k++;
	}
	if (count > 1 && str[k] != '\0')
	{
		while (count > 0)
		{
			result[*j] = '\'';
			(*j)++;
			(*i)++;
			count--;
		}
	}
}

static void	handle_single_quote_content(const char *str, char *result, int *i,
		int *j)
{
	(*i)++;
	while (str[*i] && str[*i] != '\'')
	{
		result[*j] = str[*i];
		(*j)++;
		(*i)++;
	}
	if (str[*i] == '\'')
		(*i)++;
}

static void	process_quotes(const char *str, char *result)
{
	int	val[3];

	val[0] = 0;
	val[1] = 0;
	val[2] = 0;
	while (str[val[0]])
	{
		if (str[val[0]] == '\"' && (!val[2] || val[2]))
			handle_double_quotes_rem(str, &val[0], &val[2]);
		else if (str[val[0]] == '\'' && !val[2])
		{
			if (str[val[0] + 1] == '\'')
				val[0] += 2;
			else
			{
				handle_consecutive_quotes(str, result, &val[0], &val[1]);
				if (str[val[0]] == '\'')
					handle_single_quote_content(str, result, &val[0], &val[1]);
			}
		}
		else
			result[val[1]++] = str[val[0]++];
	}
	result[val[1]] = '\0';
}

char	*remove_quotes(const char *str)
{
	char	*result;

	result = gc_malloc(strlen(str) + 1);
	if (!result)
		return (ft_strdup(""));
	process_quotes(str, result);
	return (result);
}
