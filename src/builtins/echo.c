#include "../../include/minishell.h"

static int is_n_option(const char *str) {
  if (str[0] != '-')
    return 0;
  int j = 1;
  while (str[j] == 'n')
    j++;
  return str[j] == '\0';
}

void echo(char **args) {
  int i;
  int new_line;

  new_line = 1;
  i = 1;

  while (args[i] && is_n_option(args[i])) {
    new_line = 0;
    i++;
  }

  while (args[i]) {
    ft_putstr_fd(args[i], STDOUT_FILENO);
    if (args[i + 1])
      ft_putstr_fd(" ", STDOUT_FILENO);
    i++;
  }

  if (new_line)
    ft_putstr_fd("\n", STDOUT_FILENO);
}
