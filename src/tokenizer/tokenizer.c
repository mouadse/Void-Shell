#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

// Function protoypes for error suppressing to be removed later
int is_whitespace(char c);
char *ft_strchr(const char *str, int c);

static int determine_special_tokens(char **input) {
  int token;

  token = **input;
  if (token == '\0') {
    return ('\0');
  } else if (token == '|') {
    (*input)++;
  } else if (token == '>') {
    (*input)++;
    if (**input == '>') {
      token = '+';
      (*input)++;
    }
  } else if (token == '<') {
    (*input)++;
    if (**input == '<') {
      token = '%';
      (*input)++;
    }
  } else {
    token = 'a';
  }
  return (token);
}

static void skip_tokens(char **current, char *end) {
  while (*current < end && !is_whitespace(**current) &&
         !ft_strchr("<|>", **current)) {
    if (**current == '\"') {
      (*current)++;
      while (*current < end && **current != '\"')
        (*current)++;
    } else if (**current == '\'') {
      (*current)++;
      while (*current < end && **current != '\'')
        (*current)++;
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
  ret = *s;
  ret = determine_special_tokens(&s);
  if (ret == 'a') {
    skip_tokens(&s, es);
  }
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
