/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander2.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msennane <msennane@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/07 23:39:15 by msennane          #+#    #+#             */
/*   Updated: 2024/12/19 19:29:07 by msennane         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static int	handle_double_quotes_rem(const char *str, char *result, int *i, int *j)
{
	if (str[*i] == '\"')
	{
		(*i)++;
		return (!1);
	}
	return (1);
}

static void	handle_single_quotes_sequence(const char *str, char *result,
		int *i, int *j)
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

static void	handle_single_quotes_content(const char *str, char *result,
		int *i, int *j)
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

static void	process_quotes(const char *str, char *result, int *i,
		int *j)
{
	int	in_double_quotes;

	in_double_quotes = 0;
	while (str[*i])
	{
		if (str[*i] == '\"')
			in_double_quotes = handle_double_quotes_rem(str, result, i, j);
		else if (str[*i] == '\'' && !in_double_quotes)
		{
			if (str[*i + 1] == '\'')
			{
				*i += 2;
				continue ;
			}
			handle_single_quotes_sequence(str, result, i, j);
			if (str[*i] == '\'')
				handle_single_quotes_content(str, result, i, j);
		}
		else
		{
			result[*j] = str[*i];
			(*j)++;
			(*i)++;
		}
	}
}

char	*remove_quotes(const char *str)
{
	int		i;
	int		j;
	char	*result;

	i = 0;
	j = 0;
	result = gc_malloc(strlen(str) + 1);
	if (!result)
		return (ft_strdup(""));
	process_quotes(str, result, &i, &j);
	result[j] = '\0';
	return (result);
}


