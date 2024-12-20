/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   remove_quotes.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msennane <msennane@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/20 23:34:10 by msennane          #+#    #+#             */
/*   Updated: 2024/12/20 23:43:05 by msennane         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static void	handle_double_quotes_rem(int *i, int *in_quotes)
{
	if (*in_quotes)
		*in_quotes = 0;
	else
		*in_quotes = 1;
	(*i)++;
}

static void	copy_quotes(char *result, int *i, int *j, int count)
{
	while (count > 0)
	{
		result[*j] = '\'';
		(*j)++;
		(*i)++;
		count--;
	}
}

static void	copy_quoted_content(const char *str, char *result, int *i, int *j)
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

static void	handle_single_quote(const char *str, char *result, int *i, int *j)
{
	int	count;
	int	k;

	if (str[*i + 1] == '\'')
	{
		*i += 2;
		return ;
	}
	count = 0;
	k = *i;
	while (str[k] == '\'')
	{
		count++;
		k++;
	}
	if (count > 1 && str[k])
		copy_quotes(result, i, j, count);
	else
		copy_quoted_content(str, result, i, j);
}

char	*remove_quotes(const char *str)
{
	int		i;
	int		j;
	int		in_quotes;
	char	*result;

	i = 0;
	j = 0;
	in_quotes = 0;
	result = gc_malloc(ft_strlen(str) + 1);
	if (!result)
		return (ft_strdup(""));
	while (str[i])
	{
		if (str[i] == '\"' && !in_quotes)
			handle_double_quotes_rem(&i, &in_quotes);
		else if (str[i] == '\'' && !in_quotes)
			handle_single_quote(str, result, &i, &j);
		else
			result[j++] = str[i++];
	}
	result[j] = '\0';
	return (result);
}
