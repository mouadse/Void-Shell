/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   remove_quotes.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msennane <msennane@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/20 23:34:10 by msennane          #+#    #+#             */
/*   Updated: 2024/12/20 23:58:04 by msennane         ###   ########.fr       */
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

// Function to handle single quotes
char handle_single_quote(const char *str, int *i)
{
    if (str[*i + 1] == '\'')
    {
        *i += 2;
        return '\0';
    }
    else
    {
        (*i)++;
        char c = str[*i];
        while (str[*i] && str[*i] != '\'')
            (*i)++;
        if (str[*i] == '\'')
            (*i)++;
        return c;
    }
}


// Function to handle characters
char handle_character(const char *str, int *i, int in_double_quotes)
{
    if (str[*i] == '\'' && !in_double_quotes)
        return handle_single_quote(str, i);
    else
        return str[*i];
}

int toggle_double_quotes(int in_double_quotes)
{
    return !in_double_quotes;
}

// Function to process the input string
void process_string(const char *str, char *result)
{
    int i;
    int j;
    int in_double_quotes;

    i = 0;
    j = 0;
    in_double_quotes = 0;

    while (str[i] != '\0')
    {
        if (str[i] == '\"')
            in_double_quotes = toggle_double_quotes(in_double_quotes);
        else
            result[j++] = handle_character(str, &i, in_double_quotes);
        i++;
    }
    result[j] = '\0';
}
char *remove_quotes(const char *str)
{
    char *result = gc_malloc(ft_strlen(str) + 1);
    if (!result)
        return ft_strdup("");

    process_string(str, result);
    return result;
}
