/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   remove_quotes.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msennane <msennane@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/20 23:34:10 by msennane          #+#    #+#             */
/*   Updated: 2024/12/20 23:40:24 by msennane         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static int handle_multiple_quotes_rem(char *result, int *i, int *j, int count) {
    while (count > 0) {
        result[*j] = '\'';
        (*j)++;
        (*i)++;
        count--;
    }
    return 0;
}

static int handle_quoted_content(const char *str, char *result, int *i, int *j) {
    (*i)++; // Skip opening quote
    while (str[*i] && str[*i] != '\'') {
        result[*j] = str[*i];
        (*j)++;
        (*i)++;
    }
    if (str[*i] == '\'') {
        (*i)++; // Skip closing quote
    }
    return 0;
}

static int handle_single_quotes_rem(const char *str, char *result, int *i, int *j) {
    int count = 0;
    int k = *i;

    while (str[k] == '\'') {
        count++;
        k++;
    }

    if (count > 1) {
        // Handle multiple consecutive quotes
        return handle_multiple_quotes_rem(result, i, j, count);
    } else if (count == 1 && str[k]) {
        // Handle quoted content
        return handle_quoted_content(str, result, i, j);
    }

    // Skip unmatched quote
    (*i)++;
    return 1;
}

static void handle_double_quote(int *i, int *in_quotes) {
    *in_quotes = !*in_quotes; // Toggle quote state
    (*i)++; // Move past the double quote
}

char *remove_quotes(const char *str) {
    int i = 0;
    int j = 0;
    int in_quotes = 0;
    char *result;

    result = gc_malloc(ft_strlen(str) + 1);
    if (!result)
        return ft_strdup("");

    while (str[i]) {
        if (str[i] == '\"' && !in_quotes) {
            handle_double_quote(&i, &in_quotes);
        } else if (str[i] == '\'' && !in_quotes) {
            handle_single_quotes_rem(str, result, &i, &j);
        } else {
            result[j++] = str[i++];
        }
    }
    result[j] = '\0';
    return result;
}
