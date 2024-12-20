/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   remove_quotes.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msennane <msennane@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/20 23:34:10 by msennane          #+#    #+#             */
/*   Updated: 2024/12/20 23:55:37 by msennane         ###   ########.fr       */
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
char *remove_quotes(const char *str)
{
    if (str == NULL)
        return (NULL);

    size_t len = strlen(str);
    char *result = gc_malloc(len + 1);
    if (!result)
        return (NULL);  // Changed from returning empty string to NULL for consistency

    size_t result_index = 0;
    bool in_double_quotes = false;
    size_t i = 0;

    while (i < len)
    {
        // Handle double quotes
        if (str[i] == '"')
        {
            in_double_quotes = !in_double_quotes;
        }
        // Handle single quotes when not inside double quotes
        else if (str[i] == '\'' && !in_double_quotes)
        {
            // Skip paired single quotes (keep one of them)
            if (i + 1 < len && str[i + 1] == '\'')
            {
                result[result_index++] = str[i];
                i++;
            }
            // Skip lone single quotes
        }
        // Copy all other characters
        else
        {
            result[result_index++] = str[i];
        }
        i++;
    }

    result[result_index] = '\0';
    return (result);
}
