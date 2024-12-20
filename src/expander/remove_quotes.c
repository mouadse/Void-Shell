/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   remove_quotes.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msennane <msennane@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/20 23:34:10 by msennane          #+#    #+#             */
/*   Updated: 2024/12/21 00:17:07 by msennane         ###   ########.fr       */
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


bool is_double_quote(char c) {
    return c == '\"';
}

bool is_single_quote(char c) {
    return c == '\'';
}

void handle_double_quotes_rem(const char *str, int *i, bool *in_double_quotes) {
    if (is_double_quote(str[*i]) && !*in_double_quotes) {
        *in_double_quotes = true;
        (*i)++;
    } else if (is_double_quote(str[*i]) && *in_double_quotes) {
        *in_double_quotes = false;
        (*i)++;
    }
}

void handle_single_quotes_rem(const char *str, int *i, bool in_double_quotes, char *result, int *j) {
    if (is_single_quote(str[*i]) && !in_double_quotes) {
        if (is_single_quote(str[*i + 1])) {
            *i += 2;
            return;
        }
        int count = 0;
        int k = *i;
        while (is_single_quote(str[k])) {
            count++;
            k++;
        }
        if (count > 1 && str[k] != '\0') {
            while (count > 0) {
                result[(*j)++] = '\'';
                (*i)++;
                count--;
            }
        } else {
            (*i)++;
            while (str[*i] && !is_single_quote(str[*i]))
                result[(*j)++] = str[(*i)++];
            if (is_single_quote(str[*i]))
                (*i)++;
        }
    } else {
        result[(*j)++] = str[(*i)++];
    }
}

char *remove_quotes(const char *str) {
    int i = 0;
    int j = 0;
    bool in_double_quotes = false;
    char *result = gc_malloc(ft_strlen(str) + 1);
    if (!result)
        return ft_strdup("");

    while (str[i] != '\0') {
        handle_double_quotes_rem(str, &i, &in_double_quotes);
        handle_single_quotes_rem(str, &i, in_double_quotes, result, &j);
    }
    result[j] = '\0';
    return result;
}
