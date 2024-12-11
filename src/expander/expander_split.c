#include "../../include/minishell.h"

// Function to check if a character is a separator
static bool is_separator(char c, char sep) {
  return (c == sep || c == ' ' || c == '\t' || c == '\n');
}

// Function to count the number of words, considering quotes
static int count_words(const char *s, char sep) {
  int count = 0;
  bool in_word = false;
  bool in_single_quote = false;
  bool in_double_quote = false;

  while (*s) {
    if (*s == '\'' && !in_double_quote)
      in_single_quote = !in_single_quote;
    else if (*s == '\"' && !in_single_quote)
      in_double_quote = !in_double_quote;

    if (!in_single_quote && !in_double_quote && is_separator(*s, sep)) {
      in_word = false;
    } else if (!in_word) {
      in_word = true;
      count++;
    }
    s++;
  }
  return count;
}

// Function to get the next word, considering quotes
static char *get_next_word(const char **s_ptr, char sep) {
  const char *s = *s_ptr;
  char *word;
  int len = 0;
  bool in_single_quote = false;
  bool in_double_quote = false;

  // Skip initial separators
  while (*s && is_separator(*s, sep)) {
    s++;
  }

  const char *start = s;

  while (*s) {
    if (*s == '\'' && !in_double_quote)
      in_single_quote = !in_single_quote;
    else if (*s == '\"' && !in_single_quote)
      in_double_quote = !in_double_quote;
    else if (!in_single_quote && !in_double_quote && is_separator(*s, sep))
      break;
    len++;
    s++;
  }

  word = gc_malloc(sizeof(char) * (len + 1));
  if (!word)
    return NULL;

  for (int i = 0; i < len; i++) {
    word[i] = start[i];
  }
  word[len] = '\0';

  *s_ptr = s;
  return word;
}

// Main split function
char **ft_split_beta(const char *s, char sep) {
  char **result;
  int word_count;
  int i;

  if (!s)
    return NULL;

  word_count = count_words(s, sep);
  result = gc_malloc(sizeof(char *) * (word_count + 1));
  if (!result)
    return NULL;

  i = 0;
  while (i < word_count) {
    result[i] = get_next_word(&s, sep);
    if (!result[i]) {
      return NULL;
    }
    i++;
  }
  result[i] = NULL;
  return result;
}

int is_ambiguous_redirect(const char *filename) {
  if (!filename)
    return 1;

  if (filename[0] == '\x01' && filename[1] == '\0')
    return 1;

  int has_special = 0;
  int has_spaces = 0;

  const char *ptr = filename;
  if (count_words(filename, ' ') > 1) {
    has_spaces = 1;
  }
  while (*ptr) {
    if (*ptr == '\x01') {
      has_special = 1;
    }
    ptr++;
  }
  return (has_special && has_spaces);
}
