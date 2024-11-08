#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
 * ---------------------------
 * Data Structures Definitions
 * ---------------------------
 */

/**
 * Queue structure for characters.
 * Used to build cleaned argument strings.
 */
typedef struct s_queue_char {
  char *data;
  int front;
  int rear;
  int capacity;
} t_queue_char;

/**
 * General Queue structure for strings.
 * Used to enqueue cleaned arguments.
 */
typedef struct s_queue {
  char **data;
  int front;
  int rear;
  int capacity;
} t_queue;

/**
 * Parameters structure containing environment variables and argument queue.
 */
typedef struct s_params {
  char **env_var_list; // Array of environment variable strings (e.g.,
                       // "HOME=/home/user")
  t_queue *args_queue; // Queue to store processed arguments
} t_params;

/**
 * Command types enumeration.
 */
typedef enum e_cmd_type { EXEC, PIPE, REDIR } t_cmd_type;

/**
 * Base Command structure.
 */
typedef struct s_cmd {
  t_cmd_type type;
} t_cmd;

/**
 * Execution Command structure.
 */
typedef struct s_execcmd {
  t_cmd base;
  char **argv;
} t_execcmd;

/**
 * Pipe Command structure.
 */
typedef struct s_pipecmd {
  t_cmd base;
  struct s_cmd left;
  struct s_cmd right;
} t_pipecmd;

/**
 * Redirection Command structure.
 */
typedef struct s_redircmd {
  t_cmd base;
  int r_type;           // Type of redirection
  char *file;           // File involved in redirection
  struct s_cmd *subcmd; // Sub-command
} t_redircmd;

/*
 * ---------------------------
 * Utility Function Prototypes
 * ---------------------------
 */

/* Queue Char Functions */
void init_queue_char(t_queue_char *q);
void enqueue_char(t_queue_char *q, char c);
void enqueue_str(t_queue_char *q, const char *str);
char *queue_char_to_str(t_queue_char *q);

/* Queue Functions */
void init_queue(t_queue *q);
void enqueue(t_queue *q, char *str);
char *dequeue(t_queue *q);

/* String Utility Functions */
int ft_strcmp(const char *s1, const char *s2);
char *ft_strdup(const char *s1);
char *ft_substr(const char *s, unsigned int start, size_t len);
char *ft_itoa(int n);
int ft_strlen(const char *s);
char *ft_strchr_custom(const char *s, int c);
int ft_isdigit_custom(int c);
int ft_isalnum_custom(int c);

/* Environment Variable Handling */
char *getenv_value(const char *name, char **env_var_list);

/* Memory Management */
void free1(void *ptr);

/* Whitespace Check */
int is_whitespace(char c);

/* Command Processing Functions */
void process_all_commands(t_cmd *cmd, t_params *params, int *exit_status);
static void clean_execution_command_args(t_cmd *cmd, t_params *params,
                                         int *exit_status);
int has_special_characters(const char *arg);
static char *clean_argument(const char *arg, t_params *params,
                            int *exit_status);
static void process_argument(const char *arg, t_queue_char *queue,
                             int *exit_status, t_params *params);
void handle_dollar_sign(const char *arg, int *index, t_queue_char *queue,
                        t_params *params, int *exit_status);
char *extract_variable_name(const char *arg);
void handle_double_quotes(const char *arg, int *index, t_queue_char *queue,
                          t_params *params, int *exit_status);
void handle_single_quotes(const char *arg, int *index, t_queue_char *queue);

/*
 * ---------------------------
 * Utility Function Implementations
 * ---------------------------
 */

/* Initialize character queue */
void init_queue_char(t_queue_char *q) {
  q->capacity = 1024;
  q->front = 0;
  q->rear = 0;
  q->data = (char *)malloc(q->capacity * sizeof(char));
  if (!q->data) {
    fprintf(stderr, "Memory allocation failed for char queue.\n");
    exit(EXIT_FAILURE);
  }
}

/* Enqueue a single character */
void enqueue_char(t_queue_char *q, char c) {
  if (q->rear >= q->capacity - 1) { // Leave space for null terminator
    q->capacity *= 2;
    q->data = (char *)realloc(q->data, q->capacity * sizeof(char));
    if (!q->data) {
      fprintf(stderr, "Memory reallocation failed for char queue.\n");
      exit(EXIT_FAILURE);
    }
  }
  q->data[q->rear++] = c;
}

/* Enqueue a string */
void enqueue_str(t_queue_char *q, const char *str) {
  while (*str) {
    enqueue_char(q, *str++);
  }
}

/* Convert queue to null-terminated string */
char *queue_char_to_str(t_queue_char *q) {
  enqueue_char(q, '\0'); // Null-terminate
  char *str = strdup(q->data);
  if (!str) {
    fprintf(stderr, "Memory allocation failed for queue to string.\n");
    exit(EXIT_FAILURE);
  }
  free(q->data);
  return str;
}

void free_queue_char(t_queue_char *q) {
  if (q->data)
    free(q->data);
}

/* Initialize string queue */
void init_queue(t_queue *q) {
  q->capacity = 1024;
  q->front = 0;
  q->rear = 0;
  q->data = (char **)malloc(q->capacity * sizeof(char *));
  if (!q->data) {
    fprintf(stderr, "Memory allocation failed for string queue.\n");
    exit(EXIT_FAILURE);
  }
}

/* Enqueue a string */
void enqueue(t_queue *q, char *str) {
  if (q->rear >= q->capacity) {
    q->capacity *= 2;
    q->data = (char **)realloc(q->data, q->capacity * sizeof(char *));
    if (!q->data) {
      fprintf(stderr, "Memory reallocation failed for string queue.\n");
      exit(EXIT_FAILURE);
    }
  }
  q->data[q->rear++] = str;
}

/* Dequeue a string */
char *dequeue(t_queue *q) {
  if (q->front == q->rear) {
    return NULL; // Queue is empty
  }
  return q->data[q->front++];
}

/* Custom strcmp */
int ft_strcmp(const char *s1, const char *s2) { return strcmp(s1, s2); }

/* Custom strdup */
char *ft_strdup(const char *s1) { return strdup(s1); }

/* Custom substring */
char *ft_substr(const char *s, unsigned int start, size_t len) {
  if (!s)
    return NULL;
  size_t s_len = strlen(s);
  if (start >= s_len)
    return strdup("");
  size_t max_len = s_len - start;
  if (len > max_len)
    len = max_len;
  char *substr = (char *)malloc((len + 1) * sizeof(char));
  if (!substr)
    return NULL;
  strncpy(substr, s + start, len);
  substr[len] = '\0';
  return substr;
}

/* Custom itoa */
char *ft_itoa(int n) {
  char buffer[12]; // Enough for 32-bit integers
  snprintf(buffer, sizeof(buffer), "%d", n);
  return strdup(buffer);
}

/* Custom strlen */
int ft_strlen(const char *s) { return strlen(s); }

/* Custom strchr */
char *ft_strchr_custom(const char *s, int c) { return strchr(s, c); }

/* Custom isdigit */
int ft_isdigit_custom(int c) { return isdigit(c); }

/* Custom isalnum */
int ft_isalnum_custom(int c) { return isalnum(c); }

/* Free memory */
void free1(void *ptr) { free(ptr); }

/* Check if character is whitespace */
int is_whitespace(char c) { return (c == ' ' || c == '\t' || c == '\n'); }

/* Get environment variable value from env_var_list */
char *getenv_value(const char *name, char **env_var_list) {
  size_t name_len = strlen(name);
  for (int i = 0; env_var_list[i] != NULL; i++) {
    if (strncmp(env_var_list[i], name, name_len) == 0 &&
        env_var_list[i][name_len] == '=') {
      return strdup(env_var_list[i] + name_len + 1);
    }
  }
  return NULL; // Variable not found
}

/*
 * ---------------------------
 * Command Processing Implementations
 * ---------------------------
 */

/**
 * Extracts the variable name following a dollar sign.
 * Handles numeric variables and alphanumeric variables with underscores.
 *
 * @param arg The input argument string starting after the dollar sign.
 * @return A newly allocated string containing the variable name.
 */
char *extract_variable_name(const char *arg) {
  int i = 0;

  if (ft_isdigit_custom(arg[i])) {
    // Handle numeric variables like $1, $2, etc.
    int num = arg[i] - '0';
    return ft_itoa(num);
  }

  // Handle alphanumeric variable names with underscores
  while (arg[i] && (ft_isalnum_custom(arg[i]) || arg[i] == '_')) {
    i++;
  }

  return ft_substr(arg, 0, i);
}

/**
 * Handles the processing of double-quoted segments.
 * Allows variable expansion within double quotes.
 *
 * @param arg The input argument string.
 * @param index Pointer to the current index in the argument string.
 * @param queue The character queue to enqueue processed characters.
 * @param params The parameters containing environment variables and other
 * settings.
 * @param exit_status Pointer to the exit status variable.
 */
void handle_double_quotes(const char *arg, int *index, t_queue_char *queue,
                          t_params *params, int *exit_status) {
  (*index)++; // Skip the opening double quote
  while (arg[*index] && arg[*index] != '\"') {
    if (arg[*index] == '$') {
      if (arg[*index + 1] == '?') {
        // Handle special variable $? which holds the exit status
        char *exit_status_str = ft_itoa(*exit_status);
        enqueue_str(queue, exit_status_str);
        free1(exit_status_str);
        (*index) += 2; // Skip "$?"
      } else {
        // Handle regular environment variable
        (*index)++; // Skip the dollar sign
        char *var_name = extract_variable_name(&arg[*index]);
        if (var_name && var_name[0] != '\0') {
          char *var_value = getenv_value(var_name, params->env_var_list);
          if (var_value) {
            enqueue_str(queue, var_value);
            free1(var_value);
          }
        }
        if (var_name) {
          (*index) += ft_strlen(var_name);
          free1(var_name);
        }
      }
    } else {
      // Regular character inside double quotes
      enqueue_char(queue, arg[*index]);
      (*index)++;
    }
  }
  if (arg[*index] == '\"') {
    (*index)++; // Skip the closing double quote
  }
}

/**
 * Handles the processing of single-quoted segments.
 * Treats everything within single quotes as literal characters.
 *
 * @param arg The input argument string.
 * @param index Pointer to the current index in the argument string.
 * @param queue The character queue to enqueue processed characters.
 */
void handle_single_quotes(const char *arg, int *index, t_queue_char *queue) {
  (*index)++; // Skip the opening single quote
  while (arg[*index] && arg[*index] != '\'') {
    enqueue_char(queue, arg[*index]);
    (*index)++;
  }
  if (arg[*index] == '\'') {
    (*index)++; // Skip the closing single quote
  }
}

/**
 * Processes a dollar sign in the argument string.
 * Handles variable expansion based on the following character(s).
 *
 * @param arg The input argument string.
 * @param index Pointer to the current index in the argument string.
 * @param queue The character queue to enqueue processed characters.
 * @param params The parameters containing environment variables and other
 * settings.
 * @param exit_status Pointer to the exit status variable.
 */
void handle_dollar_sign(const char *arg, int *index, t_queue_char *queue,
                        t_params *params, int *exit_status) {
  (*index)++; // Skip the dollar sign

  if (!arg[*index] || is_whitespace(arg[*index])) {
    // If dollar sign is at the end or followed by whitespace, treat it as a
    // literal '$'
    enqueue_char(queue, '$');
    return;
  }

  if (arg[*index] == '?') {
    // Handle special variable $? which holds the exit status
    char *exit_status_str = ft_itoa(*exit_status);
    enqueue_str(queue, exit_status_str);
    free1(exit_status_str);
    (*index)++; // Skip '?'
  } else {
    // Handle regular environment variable
    char *var_name = extract_variable_name(&arg[*index]);
    if (var_name && var_name[0] != '\0') {
      char *var_value = getenv_value(var_name, params->env_var_list);
      if (var_value) {
        enqueue_str(queue, var_value);
        free1(var_value);
      }
    }
    if (var_name) {
      (*index) += ft_strlen(var_name);
      free1(var_name);
    }
  }
}

/**
 * Processes the input argument string by handling quotes and special
 * characters. Builds a clean argument string with proper expansions.
 *
 * @param arg The input argument string to clean.
 * @param queue The character queue to build the clean argument.
 * @param exit_status Pointer to the exit status variable.
 * @param params The parameters containing environment variables and other
 * settings.
 */
static void process_argument(const char *arg, t_queue_char *queue,
                             int *exit_status, t_params *params) {
  int i = 0;

  while (arg[i]) {
    if (arg[i] == '\'') {
      // Handle single-quoted string
      handle_single_quotes(arg, &i, queue);
    } else if (arg[i] == '\"') {
      // Handle double-quoted string with possible variable expansion
      handle_double_quotes(arg, &i, queue, params, exit_status);
    } else if (arg[i] == '$') {
      // Handle variable expansion outside of quotes
      handle_dollar_sign(arg, &i, queue, params, exit_status);
    } else {
      // Regular character, enqueue as-is
      enqueue_char(queue, arg[i++]);
    }
  }
}

/**
 * Cleans the input argument by handling special characters and quotes.
 * If the argument is a tilde (~), it replaces it with the HOME environment
 * variable.
 *
 * @param arg The input argument string to clean.
 * @param params The parameters containing environment variables and other
 * settings.
 * @param exit_status Pointer to the exit status variable.
 * @return A newly allocated string containing the cleaned argument.
 */
/**
 * Cleans the input argument by handling special characters and quotes.
 * If the argument is a tilde (~), it replaces it with the HOME environment
 * variable.
 *
 * @param arg The input argument string to clean.
 * @param params The parameters containing environment variables and other
 * settings.
 * @param exit_status Pointer to the exit status variable.
 * @return A newly allocated string containing the cleaned argument.
 */
static char *clean_argument(const char *arg, t_params *params,
                            int *exit_status) {
  t_queue_char queue;
  char *clean_arg;
  // int queue_flag = 0;
  init_queue_char(&queue);

  if (ft_strcmp(arg, "~") == 0) {
    // Replace tilde (~) with the value of HOME environment variable
    char *home = getenv_value("HOME", params->env_var_list);
    if (home) {
      clean_arg = home;
    } else {
      clean_arg = ft_strdup("~"); // Fallback to "~" if HOME is not set
    }
    free_queue_char(&queue);
  } else {
    // Process the argument to handle quotes and special characters
    process_argument(arg, &queue, exit_status, params);
    // Convert the character queue to a string
    clean_arg = queue_char_to_str(&queue);
  }

  // Enqueue the cleaned argument for further processing
  enqueue(params->args_queue, clean_arg);
  // if (queue_flag == 1) {
  //   free_queue_char(&queue);
  // }
  return clean_arg;
}

/**
 * Checks if the argument contains any special characters that require
 * processing.
 *
 * @param arg The input argument string to check.
 * @return 1 if special characters are found, 0 otherwise.
 */
int has_special_characters(const char *arg) {
  return (ft_strchr_custom(arg, '\'') || ft_strchr_custom(arg, '\"') ||
          ft_strchr_custom(arg, '$') || ft_strchr_custom(arg, '~'))
             ? 1
             : 0;
}

/**
 * Cleans all arguments in an execution command by handling special characters.
 *
 * @param cmd The execution command containing arguments.
 * @param params The parameters containing environment variables and other
 * settings.
 * @param exit_status Pointer to the exit status variable.
 */
static void clean_execution_command_args(t_cmd *cmd, t_params *params,
                                         int *exit_status) {
  int i = 0;
  t_execcmd *exec_cmd = (t_execcmd *)cmd;

  while (exec_cmd->argv[i]) {
    char *cleaned_arg;
    if (has_special_characters(exec_cmd->argv[i])) {
      // Process the argument and enqueue the cleaned version
      cleaned_arg = clean_argument(exec_cmd->argv[i], params, exit_status);
    } else {
      // Duplicate and enqueue the original argument
      cleaned_arg = ft_strdup(exec_cmd->argv[i]);
      enqueue(params->args_queue, cleaned_arg);
    }
    // Do not modify exec_cmd->argv[i]
    i++;
  }
}

/**
 * Recursively processes all commands, handling pipes and redirections.
 *
 * @param cmd The current command to process.
 * @param params The parameters containing environment variables and other
 * settings.
 * @param exit_status Pointer to the exit status variable.
 */
void process_all_commands(t_cmd *cmd, t_params *params, int *exit_status) {
  if (cmd->type == PIPE) {
    // If the command is a pipe, process both left and right commands
    t_pipecmd *pipe_cmd = (t_pipecmd *)cmd;
    process_all_commands(&pipe_cmd->left, params, exit_status);
    process_all_commands(&pipe_cmd->right, params, exit_status);
  } else if (cmd->type == REDIR) {
    // If the command involves redirection, process the file and the subcommand
    t_redircmd *redir_cmd = (t_redircmd *)cmd;
    if (redir_cmd->r_type != '%' && has_special_characters(redir_cmd->file)) {
      redir_cmd->file = clean_argument(redir_cmd->file, params, exit_status);
    }
    process_all_commands(redir_cmd->subcmd, params, exit_status);
  } else if (cmd->type == EXEC) {
    // If the command is an execution command, clean its arguments
    clean_execution_command_args(cmd, params, exit_status);
  }
}

/*
 * ---------------------------
 * Main Function for Unit Testing (Corrected)
 * ---------------------------
 */

/*
 * ---------------------------
 * Main Function for Unit Testing (Corrected)
 * ---------------------------
 */

int main() {
  // Initialize environment variables
  char *env_vars[] = {"HOME=/home/user", "USER=user", "SHELL=/bin/bash",
                      "PATH=/usr/bin:/bin", NULL};

  // Initialize parameters
  t_params params;
  params.env_var_list = env_vars;

  // Initialize argument queue
  t_queue args_queue;
  init_queue(&args_queue);
  params.args_queue = &args_queue;

  // Define test cases
  const char *test_cases[] = {
      "simpleArg",
      "'single quoted arg'",
      "\"double quoted arg with $USER\"",
      "mixed 'single $USER' and \"double $HOME\" quotes",
      "argumentWith$HOMEVariable",
      "argumentWith$?ExitStatus",
      "~",
      "path/to/some~file",
      "$HOME/$USER/docs",
      "\"Nested 'quotes' and $SHELL\"",
      "noSpecialChars",
      NULL};

  // Define a mock exit status
  int exit_status = 0;

  // Process each test case
  printf("---- Argument Processing Unit Tests ----\n\n");
  for (int i = 0; test_cases[i] != NULL; i++) {
    printf("Test Case %d: \"%s\"\n", i + 1, test_cases[i]);

    // Create an execution command
    t_execcmd exec_cmd;
    exec_cmd.base.type = EXEC;

    // For simplicity, each test case is a single argument
    exec_cmd.argv = (char **)malloc(2 * sizeof(char *));
    if (!exec_cmd.argv) {
      fprintf(stderr, "Memory allocation failed for exec_cmd.argv.\n");
      exit(EXIT_FAILURE);
    }
    exec_cmd.argv[0] = ft_strdup(test_cases[i]);
    exec_cmd.argv[1] = NULL;

    // Process the command
    process_all_commands((t_cmd *)&exec_cmd, &params, &exit_status);

    // Dequeue and print the processed arguments
    printf("Processed Arguments:\n");
    char *processed_arg;
    while ((processed_arg = dequeue(&args_queue)) != NULL) {
      printf("  %s\n", processed_arg);
      free1(processed_arg);
    }
    printf("\n");

    // Free the original argument
    free(exec_cmd.argv[0]);
    free(exec_cmd.argv);
  }

  // Free the string queue data
  free(args_queue.data);

  printf("---- End of Tests ----\n");
  return 0;
}
