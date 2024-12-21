/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   remove_quotes.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msennane <msennane@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/20 23:34:10 by msennane          #+#    #+#             */
/*   Updated: 2024/12/21 14:34:02 by msennane         ###   ########.fr       */
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
		(*i)++;
		return (1);
	}
	return (0);
}

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
		result[j++] = str[i++];
	}
	result[j] = '\0';
	return (result);
}
