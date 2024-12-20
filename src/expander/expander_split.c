/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander_split.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msennane <msennane@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/20 23:22:53 by msennane          #+#    #+#             */
/*   Updated: 2024/12/21 00:55:13 by msennane         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static bool	is_separator(char c, char sep)
{
	return (c == sep || c == ' ' || c == '\t' || c == '\n');
}

static int	count_words(const char *s, char sep)
{
	int		count;
	bool	in_word;
	bool	in_single_quote;
	bool	in_double_quote;

	count = 0;
	in_word = false;
	in_single_quote = false;
	in_double_quote = false;
	while (*s)
	{
		if (*s == '\'' && !in_double_quote)
			in_single_quote = !in_single_quote;
		else if (*s == '\"' && !in_single_quote)
			in_double_quote = !in_double_quote;
		if (!in_single_quote && !in_double_quote && is_separator(*s, sep))
			in_word = false;
		else if (!in_word)
		{
			in_word = true;
			count++;
		}
		s++;
	}
	return (count);
}

static char	*get_next_word(const char **s_ptr, char sep)
{
	const char	*s;
	char		*word;
	int			len;
	bool		in_single_quote;
	bool		in_double_quote;
	const char	*start;

	s = *s_ptr;
	len = 0;
	in_single_quote = false;
	in_double_quote = false;
	while (*s && is_separator(*s, sep))
		s++;
	start = s;
	while (*s)
	{
		if (*s == '\'' && !in_double_quote)
			in_single_quote = !in_single_quote;
		else if (*s == '\"' && !in_single_quote)
			in_double_quote = !in_double_quote;
		else if (!in_single_quote && !in_double_quote && is_separator(*s, sep))
			break ;
		len++;
		s++;
	}
	word = gc_malloc(sizeof(char) * (len + 1));
	if (!word)
		return (NULL);
	int i = 0;
	while (i < len)
	{
		word[i] = start[i];
		i++;
	}
	word[len] = '\0';
	*s_ptr = s;
	return (word);
}

char	**ft_split_beta(const char *s, char sep)
{
	char	**result;
	int		word_count;
	int		i;

	if (!s)
		return (NULL);
	word_count = count_words(s, sep);
	result = gc_malloc(sizeof(char *) * (word_count + 1));
	if (!result)
		return (NULL);
	i = 0;
	while (i < word_count)
	{
		result[i] = get_next_word(&s, sep);
		if (!result[i])
		{
			return (NULL);
		}
		i++;
	}
	result[i] = NULL;
	return (result);
}

int	is_ambiguous_redirect(const char *filename)
{
	if (!filename || filename[0] == '\0')
	{
		return (1);
	}
	if (ft_strchr(filename, '\x01'))
	{
		return (1);
	}
	if (count_words(filename, ' ') > 1)
	{
		return (1);
	}
	return (0);
}
