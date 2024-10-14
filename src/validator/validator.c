#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
// Function prototypes

int validate_cmd(const char *buf, int *exit_code);

// Helper functions prototypes

static int has_unbalanced_quotes(const char *buf);
static int is_redirs(char c);
static int is_pipe(char c);
static int is_whitespace(char x);

int validate_cmd(const char *buf, int *exit_code) {
  int i;
  char curr_quote;

  i = 0;
  curr_quote = '\0';

  // 1. First part : checking for unbalanced quotes in our buffer
  while (buf[i] != '\0') {
    if (curr_quote) {
      if (buf[i] == curr_quote)
        curr_quote = '\0';
    } else {
      if (buf[i] == '\"' || buf[i] == '\'')
        curr_quote = buf[i];
    }
    i++;
  }
  if (curr_quote) {
    fprintf(stderr, "Syntax error: Unbalanced quote '%c'\n", curr_quote);
    *exit_code = 258;
    return (0); // Invalid due to unbalanced quotes
  }
  // 2. 2nd check. Validate syntax for pipes and redirections
  i = 0;
  int len = strlen(buf);
  int expecting_command = 1;
  while (i < len) {
    // Skip whitespaces
    while (i < len && is_whitespace(buf[i]))
      i++;
    if (i >= len) // to avoid bad errors
      break;
    char c = buf[i];
    // Check for pipe in our buf
    if (is_pipe(c)) {
      if (expecting_command) {
        fprintf(
            stderr,
            "Syntax err: PIPE '|' cannot start a cmd or follow another pipe\n");
        *exit_code = 258;
        return (0);
      }
      expecting_command = 1;
      i++;
      continue;
    }
    // Check for redirections
    if (is_redirs(c)) {
      if (expecting_command) {
        fprintf(stderr, "Error: Redirection '%c' cannot start a command\n", c);
        *exit_code = 258;
        return (0);
      }
      // skip redir chars
      while (i < len && is_redirs(buf[i]))
        i++;
      // skip whitespace after redirs
      while (i < len && is_whitespace(buf[i]))
        i++;
      if (i >= len) {
        fprintf(
            stderr,
            "Error: Redirection '%c' must be followed by a file or command\n",
            c);
        *exit_code = 258;
        return (0); // redirection without a target
      }
      // Ensure the next token is a valid arg (simplified as a non-special
      // character)
      if (buf[i] == '|' || is_redirs(buf[i])) {
        fprintf(stderr, "Error: Invalid token '%c' after redirection\n",
                buf[i]);
        *exit_code = 258;
        return (0);
      }
      expecting_command = 0;
      continue;
    }
    while (i < len && !is_whitespace(buf[i]) && !is_pipe(buf[i]) &&
           !is_redirs(buf[i]))
      i++;
    expecting_command = 0; // command or argument processed
  }
  // Final check: command should not end with a pipe
  if (len > 0 && buf[len - 1] == '|') {
    fprintf(stderr, "Error: Command cannot end with a pipe '|'\n");
    *exit_code = 258;
    return 0; // Invalid ending with pipe
  }
  return 1; // Command is valid
}

static int is_whitespace(char x) {
  return (x == ' ' || x == '\t' || x == '\n' || x == '\r' || x == '\v');
}

static int is_pipe(char c) { return (c == '|'); }

static int is_redirs(char c) {
  return (c == '>' || c == '<' || c == '+' || c == '%');
}

// int main(void) {
//   const char *command = "echo \"Hello World\" | grep Hello > output.txt";
//   int exit_status = 0;
//
//   if (validate_cmd(command, &exit_status)) {
//     printf("Command is valid.\n");
//   } else {
//     printf("Command is invalid. Exit status: %d\n", exit_status);
//   }
//   return (EXIT_SUCCESS);
// }

int main(void) {
  int exit_code = 0;
  int tests_run = 0;
  int tests_passed = 0;

  // Test case 1: Valid simple command
  if (validate_cmd("ls -l", &exit_code) == 1 && exit_code == 0) {
    printf("Test 1 passed: Valid simple command\n");
    tests_passed++;
  } else {
    printf("Test 1 failed: Valid simple command\n");
  }
  tests_run++;

  // Test case 2: Valid command with pipe
  if (validate_cmd("echo \"Hello World\" | grep Hello", &exit_code) == 1 &&
      exit_code == 0) {
    printf("Test 2 passed: Valid command with pipe\n");
    tests_passed++;
  } else {
    printf("Test 2 failed: Valid command with pipe\n");
  }
  tests_run++;

  // Test case 3: Valid command with redirection
  if (validate_cmd("echo \"Hello\" > output.txt", &exit_code) == 1 &&
      exit_code == 0) {
    printf("Test 3 passed: Valid command with redirection\n");
    tests_passed++;
  } else {
    printf("Test 3 failed: Valid command with redirection\n");
  }
  tests_run++;

  // Test case 4: Unbalanced quotes
  if (validate_cmd("echo \"Hello", &exit_code) == 0 && exit_code == 258) {
    printf("Test 4 passed: Unbalanced quotes\n");
    tests_passed++;
  } else {
    printf("Test 4 failed: Unbalanced quotes\n");
  }
  tests_run++;

  // Test case 5: Invalid pipe usage
  if (validate_cmd("| echo Hello", &exit_code) == 0 && exit_code == 258) {
    printf("Test 5 passed: Invalid pipe usage\n");
    tests_passed++;
  } else {
    printf("Test 5 failed: Invalid pipe usage\n");
  }
  tests_run++;

  // Test case 6: Invalid redirection usage
  if (validate_cmd("> output.txt echo Hello", &exit_code) == 0 &&
      exit_code == 258) {
    printf("Test 6 passed: Invalid redirection usage\n");
    tests_passed++;
  } else {
    printf("Test 6 failed: Invalid redirection usage\n");
  }
  tests_run++;

  // Test case 7: Command ending with pipe
  if (validate_cmd("echo Hello |", &exit_code) == 0 && exit_code == 258) {
    printf("Test 7 passed: Command ending with pipe\n");
    tests_passed++;
  } else {
    printf("Test 7 failed: Command ending with pipe\n");
  }
  tests_run++;

  // Test case 8: Empty command
  if (validate_cmd("", &exit_code) == 1 && exit_code == 0) {
    printf("Test 8 passed: Empty command\n");
    tests_passed++;
  } else {
    printf("Test 8 failed: Empty command\n");
  }
  tests_run++;

  // Print test results summary
  printf("\nTest Results: %d/%d tests passed\n", tests_passed, tests_run);

  return (tests_passed == tests_run) ? EXIT_SUCCESS : EXIT_FAILURE;
}
