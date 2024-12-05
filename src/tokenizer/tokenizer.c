/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msennane <msennane@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/06 20:18:35 by msennane          #+#    #+#             */
/*   Updated: 2024/12/05 13:24:14 by msennane         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static void	determine_special_tokens(char **input, int *token)
{
	if (**input == '\0')
		*token = '\0';
	else if (**input == '|')
	{
		*token = '|';
		(*input)++;
	}
	else if (**input == '>')
	{
		(*input)++;
		if (**input == '>')
		{
			*token = '+';
			(*input)++;
		}
		else
			*token = '>';
	}
	else if (**input == '<')
	{
		(*input)++;
		if (**input == '<')
		{
			*token = '%';
			(*input)++;
		}
		else
			*token = '<';
	}
	else
		*token = 'a';
}

static void	skip_tokens(char **current, char *end)
{
	while (*current < end && !is_whitespace(**current) && !ft_strchr("<|>",
			**current))
	{
		if (**current == '\"')
		{
			(*current)++;
			while (*current < end && **current != '\"')
				(*current)++;
			if (*current >= end)
				break ;
		}
		else if (**current == '\'')
		{
			(*current)++;
			while (*current < end && **current != '\'')
				(*current)++;
			if (*current >= end)
				break ;
		}
		if (*current < end)
			(*current)++;
	}
}

int	gettoken(char **ps, char *es, char **q, char **eq)
{
	char	*s;
	int		ret;

	s = *ps;
	while (s < es && is_whitespace(*s))
		s++;
	if (q)
		*q = s;
	determine_special_tokens(&s, &ret);
	if (ret == 'a')
		skip_tokens(&s, es);
	if (eq)
		*eq = s;
	*ps = s;
	return (ret);
}

int	peek(char **ps, char *es, char *toks)
{
	char	*s;

	s = *ps;
	while (s < es && is_whitespace(*s))
		s++;
	return (*s && ft_strchr(toks, *s));
}
