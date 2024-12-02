/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msennane <msennane@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/06 20:18:35 by msennane          #+#    #+#             */
/*   Updated: 2024/12/02 12:38:29 by msennane         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static int determine_special_tokens(char **input) {
  int token;
  token = **input;
  if (token == '\0')
    return ('\0');
  else if (token == '|') {
    (*input)++;
    return token;
  } else if (token == '>') {
    (*input)++;
    if (**input == '>') {
      (*input)++;
      return '+'; // Double '>' token
    }
    return token; // Single '>' token
  } else if (token == '<') {
    (*input)++;
    if (**input == '<') {
      (*input)++;
      return '%'; // Double '<' token
    }
    return token; // Single '<' token
  } else
    return 'a';
}
static void skip_tokens(char **current, char *end) {
  while (*current < end && !is_whitespace(**current) &&
         !ft_strchr("<|>", **current)) {
    if (**current == '\"') {
      (*current)++;
      while (*current < end && **current != '\"')
        (*current)++;
      if (*current >= end)
        break; // Unmatched quote, stop processing
    } else if (**current == '\'') {
      (*current)++;
      while (*current < end && **current != '\'')
        (*current)++;
      if (*current >= end)
        break; // Unmatched quote, stop processing
    }
    if (*current < end)
      (*current)++;
  }
}
int gettoken(char **ps, char *es, char **q, char **eq) {
  char *s;
  int ret;

  s = *ps;
  while (s < es && is_whitespace(*s))
    s++;
  if (q)
    *q = s;
  // ret = *s; dead assignment here
  ret = determine_special_tokens(&s);
  if (ret == 'a')
    skip_tokens(&s, es);
  if (eq)
    *eq = s;
  *ps = s;
  return (ret);
}

int peek(char **ps, char *es, char *toks) {
  char *s;

  s = *ps;
  while (s < es && is_whitespace(*s))
    s++;
  return (*s && ft_strchr(toks, *s));
}
