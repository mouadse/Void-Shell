/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   remove_quotes.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msennane <msennane@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/20 23:34:10 by msennane          #+#    #+#             */
/*   Updated: 2024/12/21 14:29:03 by msennane         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

// char    *remove_quotes(const char *str)
// {
//     int        i;
//     int        j;
//     int        in_double_quotes;
//     char    *result;
//     int        count;
//     int        k;

//     i = 0;
//     j = 0;
//     in_double_quotes = 0;
//     result = gc_malloc(ft_strlen(str) + 1);
//     if (!result)
//         return (ft_strdup(""));
//     while (str[i] != '\0')
//     {
//         if (str[i] == '\"' && !in_double_quotes)
//         {
//             in_double_quotes = 1;
//             i++;
//         }
//         else if (str[i] == '\"' && in_double_quotes)
//         {
//             in_double_quotes = 0;
//             i++;
//         }
//         else if (str[i] == '\'' && !in_double_quotes)
//         {
//             if (str[i + 1] == '\'')
//             {
//                 i += 2;
//                 continue ;
//             }
//             count = 0;
//             k = i;
//             while (str[k] == '\'')
//             {
//                 count++;
//                 k++;
//             }
//             if (count > 1 && str[k] != '\0')
//             {
//                 while (count > 0)
//                 {
//                     result[j++] = '\'';
//                     i++;
//                     count--;
//                 }
//             }
//             else
//             {
//                 i++;
//                 while (str[i] && str[i] != '\'')
//                     result[j++] = str[i++];
//                 if (str[i] == '\'')
//                     i++;
//             }
//         }
//         else
//             result[j++] = str[i++];
//     }
//     result[j] = '\0';
//     return (result);
// }

static int	process_double_quote(const char *str, int *i, int *in_double_quotes)
{
	if (str[*i] == '\"')
	{
		*in_double_quotes = !(*in_double_quotes);
		(*i)++; // Move past the quote
		return (1);
	}
	return (0);
}

/**
 * Handles the case when we encounter single quotes *outside* of double quotes.
 * This handles two scenarios:
 *   1. Two consecutive single quotes (like `''`) which we skip entirely.
 *   2. A block of single quotes and the content inside them.
 *
 * @param str    The entire input string.
 * @param i      Pointer to current index in the string.
 * @param j      Pointer to the index for the 'result' buffer.
 * @param result The buffer where we write the output string.
 */
static void	process_single_quote(const char *str, int *i, int *j, char *result)
{
	int	count;

	if (str[*i + 1] == '\'')
	{
		*i += 2;
		return ;
	}
	count = 0;
	while (str[*i + count] == '\'')
		count++;
	if (count > 1 && str[*i + count] != '\0')
	{
		while (count--)
			result[(*j)++] = str[(*i)++];
	}
	else
	{
		(*i)++;
		while (str[*i] && str[*i] != '\'')
			result[(*j)++] = str[(*i)++];
		if (str[*i] == '\'')
			(*i)++;
	}
}
/**
 * The main remove_quotes function that orchestrates the logic.
 */
char	*remove_quotes(const char *str)
{
	int		i;
	int		j;
	int		in_double_quotes;
	char	*result;

	i = 0;
	j = 0;
	in_double_quotes = 0;
	result = gc_malloc(ft_strlen(str) + 1);
	if (!result)
		return (ft_strdup(""));
	while (str[i] != '\0')
	{
		if (process_double_quote(str, &i, &in_double_quotes))
			continue ;
		if (!in_double_quotes && str[i] == '\'')
		{
			process_single_quote(str, &i, &j, result);
			continue ;
		}
		result[j] = str[i];
		i++;
		j++;
	}
	result[j] = '\0';
	return (result);
}
