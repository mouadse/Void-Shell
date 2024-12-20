/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   remove_quotes.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msennane <msennane@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/20 23:34:10 by msennane          #+#    #+#             */
/*   Updated: 2024/12/20 23:46:39 by msennane         ###   ########.fr       */
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



// Helper structure to pass result buffer and index
typedef struct {
    char *buffer;
    int index;
} ResultBuffer;

// Function to handle double quotes
static int handle_double_quotes_rem(const char *str, int i, int *in_double_quotes) {
    if (str[i] == '\"') {
        *in_double_quotes = !(*in_double_quotes);
        return 1;
    }
    return 0;
}

// Function to handle single quotes
static int handle_single_quotes_core(const char *str, int *i, ResultBuffer *result_buf) {
    int start_i = *i;
    (*i)++; // Move past the initial single quote
    while (str[*i] && str[*i] != '\'') {
        result_buf->buffer[result_buf->index++] = str[*i];
        (*i)++;
    }
    if (str[*i] == '\'') {
        (*i)++;
    }
    return *i - start_i;
}

static int handle_single_quotes_rem(const char *str, int i, ResultBuffer *result_buf, int in_double_quotes) {
    if (str[i] == '\'' && !in_double_quotes) {
        if (str[i + 1] == '\'') {
            return 2; // Skip paired single quotes
        } else {
            return handle_single_quotes_core(str, (int*)&i, result_buf);
        }
    }
    return 0;
}

// Function to copy regular characters
static void copy_regular_char(const char *str, int i, ResultBuffer *result_buf) {
    result_buf->buffer[result_buf->index++] = str[i];
}

char *remove_quotes(const char *str) {
    int        i = 0;
    int        in_double_quotes = 0;
    char    *result_str;
    ResultBuffer result_buf;

    result_str = gc_malloc(ft_strlen(str) + 1);
    if (!result_str)
        return (ft_strdup(""));

    result_buf.buffer = result_str;
    result_buf.index = 0;

    while (str[i] != '\0') {
        int processed = handle_double_quotes_rem(str, i, &in_double_quotes);
        if (processed) {
            i += processed;
            continue;
        }

        processed = handle_single_quotes_rem(str, i, &result_buf, in_double_quotes);
        if (processed > 0) {
            i += processed;
            continue;
        }

        copy_regular_char(str, i, &result_buf);
        i++;
    }

    result_buf.buffer[result_buf.index] = '\0';
    return (result_buf.buffer);
}
