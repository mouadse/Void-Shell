#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define PIPE 1
#define REDIR 2
#define EXEC 3

typedef struct s_queue_char {
  char *data;
  int size;
  int capacity;
} t_queue_char;

typedef struct s_env_var {
  char *name;
  char *value;
  struct s_env_var *next;
} t_env_var;

typedef struct s_params {
  t_env_var *env_var_list;
} t_params;

// Utility Functions

void init_queue_char(t_queue_char *q) {
  q->capacity = 64;
  q->size = 0;
  q->data = malloc(q->capacity * sizeof(char));
  if (!q->data) {
    perror("malloc");
    exit(1);
  }
}

void enqueue_char(t_queue_char *q, char c) {
  if (q->size >= q->capacity) {
    q->capacity *= 2;
    q->data = realloc(q->data, q->capacity * sizeof(char));
    if (!q->data) {
      perror("realloc");
      exit(1);
    }
  }
  q->data[q->size++] = c;
}

void enqueue_str(t_queue_char *q, const char *str) {
  while (*str) {
    enqueue_char(q, *str++);
  }
}

char *queue_char_to_str(t_queue_char *q) {
  char *str = malloc((q->size + 1) * sizeof(char));
  if (!str) {
    perror("malloc");
    exit(1);
  }
  memcpy(str, q->data, q->size);
  str[q->size] = '\0';
  return str;
}

char *ft_strdup(const char *s) {
  char *dup = strdup(s);
  if (!dup) {
    perror("strdup");
    exit(1);
  }
  return dup;
}

int ft_strcmp(const char *s1, const char *s2) { return strcmp(s1, s2); }

size_t ft_strlen(const char *s) { return strlen(s); }

char *ft_substr(const char *s, unsigned int start, size_t len) {
  if (!s)
    return NULL;
  size_t slen = ft_strlen(s);
  if (start >= slen)
    return ft_strdup("");
  if (len > slen - start)
    len = slen - start;
  char *substr = malloc(len + 1);
  if (!substr) {
    perror("malloc");
    exit(1);
  }
  memcpy(substr, s + start, len);
  substr[len] = '\0';
  return substr;
}

int ft_isdigit(int c) { return isdigit(c); }

int ft_isalnum(int c) { return isalnum(c); }

char *ft_itoa(int n) {
  char buf[12]; // Enough for int32
  snprintf(buf, sizeof(buf), "%d", n);
  return ft_strdup(buf);
}

char *ft_strchr(const char *s, int c) { return strchr(s, c); }

int is_whitespace(char c) { return c == ' ' || c == '\t' || c == '\n'; }

void free1(void *ptr) { free(ptr); }

char *getenv_value(const char *name, t_env_var *env_list) {
  // For simplicity, use actual getenv
  // In practice, you might use the env_list linked list
  char *value = getenv(name);
  if (value)
    return value;
  else
    return "";
}

// Function Declarations
void process_single_quote(char *arg, int *i, t_queue_char *q);
void process_double_quote(int *values[2], char *arg, t_queue_char *q,
                          t_params *params);
void process_dollar_sign(int *values[2], t_queue_char *q, char *arg,
                         t_params *params);
int contains_special_char(char *arg);
char *getvar_name(char *arg);

// Function Implementations

static void get_clean_arg_2(char *arg, t_queue_char *q, int *exit_status,
                            t_params *params) {
  int i;
  int *values[2];

  i = 0;
  while (arg[i]) {
    if (arg[i] == '\'')
      process_single_quote(arg, &i, q);
    else if (arg[i] == '\"') {
      values[0] = &i;
      values[1] = exit_status;
      process_double_quote(values, arg, q, params);
    } else if (arg[i] == '$') {
      values[0] = &i;
      values[1] = exit_status;
      process_dollar_sign(values, q, arg, params);
    } else if (arg[i] == '~' && (i == 0 || is_whitespace(arg[i - 1]))) {
      // Tilde expansion
      char *home = getenv_value("HOME", params->env_var_list);
      if (home)
        enqueue_str(q, home);
      else
        enqueue_char(q, '~'); // If HOME is not set, leave ~ as is
      i++;
    } else
      enqueue_char(q, arg[i++]);
  }
}

static char *get_clean_arg(char *arg, t_params *params, int *exit_status) {
  t_queue_char q;
  char *clean_arg;

  init_queue_char(&q);
  if (ft_strcmp(arg, "~") == 0) {
    clean_arg = ft_strdup(getenv_value("HOME", params->env_var_list));
    free(q.data);
    return (clean_arg);
  }
  get_clean_arg_2(arg, &q, exit_status, params);
  clean_arg = queue_char_to_str(&q);
  free(q.data);
  return (clean_arg);
}

int contains_special_char(char *arg) {
  if (ft_strchr(arg, '\'') || ft_strchr(arg, '\"') || ft_strchr(arg, '$') ||
      ft_strchr(arg, '~'))
    return (1);
  return (0);
}

char *getvar_name(char *arg) {
  int i;

  i = 0;
  if (ft_isdigit(arg[i])) {
    return ft_substr(arg, i, 1);
  }
  while (arg[i] && ((ft_isalnum(arg[i]) || arg[i] == '_')))
    i++;
  return (ft_substr(arg, 0, i));
}

void process_single_quote(char *arg, int *i, t_queue_char *q) {
  (*i)++;
  while (arg[*i] && arg[*i] != '\'') {
    enqueue_char(q, arg[*i]);
    (*i)++;
  }
  if (arg[*i] == '\'')
    (*i)++;
}

void process_double_quote_2(int *values[2], char *arg, t_queue_char *q,
                            t_params *params) {
  char *var_name;
  char *var_value;
  int *i;

  i = values[0];
  (*i)++;
  var_name = getvar_name(arg + *i);
  var_value = getenv_value(var_name, params->env_var_list);
  if (var_value)
    enqueue_str(q, var_value);
  (*i) += ft_strlen(var_name);
  free1(var_name);
}

void process_double_quote_3(int *values[2], t_queue_char *q) {
  char *exit_status_str;
  int *i;
  int *exit_status;

  i = values[0];
  exit_status = values[1];
  exit_status_str = ft_itoa(*exit_status);
  enqueue_str(q, exit_status_str);
  free(exit_status_str);
  (*i) += 2;
}

void process_double_quote(int *values[2], char *arg, t_queue_char *q,
                          t_params *params) {
  int *i;

  i = values[0];
  (*i)++;
  while (arg[*i] && arg[*i] != '\"') {
    if (arg[*i] == '$' && (is_whitespace(arg[*i + 1]) || arg[*i + 1] == '\"')) {
      enqueue_char(q, '$');
      (*i)++;
    } else if (arg[*i] == '$' && arg[*i + 1] == '?')
      process_double_quote_3(values, q);
    else if (arg[*i] == '$')
      process_double_quote_2(values, arg, q, params);
    else
      enqueue_char(q, arg[(*i)++]);
  }
  if (arg[*i] == '\"')
    (*i)++;
}

void process_dollar_sign_2(int *values[2], t_queue_char *q, char *arg,
                           t_params *params) {
  char *var_name;
  char *var_value;
  int *i;

  i = values[0];
  var_name = getvar_name(arg + *i);
  var_value = getenv_value(var_name, params->env_var_list);
  if (var_value)
    enqueue_str(q, var_value);
  (*i) += ft_strlen(var_name);
  free1(var_name);
}

void process_dollar_sign(int *values[2], t_queue_char *q, char *arg,
                         t_params *params) {
  char *exit_status_str;
  int *i;
  int *exit_status;

  i = values[0];
  exit_status = values[1];
  (*i)++;
  if (!arg[*i] || is_whitespace(arg[*i])) {
    enqueue_char(q, '$');
    return;
  } else if (arg[*i] == '?') {
    exit_status_str = ft_itoa(*exit_status);
    enqueue_str(q, exit_status_str);
    free(exit_status_str);
    (*i)++;
  } else
    process_dollar_sign_2(values, q, arg, params);
}

// Main Function for Testing

int main(int argc, char *argv[]) {
  t_params params;
  int exit_status = 0; // Example exit status

  // Initialize params
  params.env_var_list = NULL; // Not used in our simplified version

  if (argc < 2) {
    printf("Usage: %s [string to process]\n", argv[0]);
    return 1;
  }

  // Process the argument
  char *clean_arg = get_clean_arg(argv[1], &params, &exit_status);

  // Print the result
  printf("Original argument: %s\n", argv[1]);
  printf("Processed argument: %s\n", clean_arg);

  // Clean up
  free(clean_arg);

  return 0;
}
