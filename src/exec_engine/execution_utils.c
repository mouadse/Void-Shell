/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msennane <msennane@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/01 23:46:26 by msennane          #+#    #+#             */
/*   Updated: 2024/11/03 16:38:20 by msennane         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void clean_empty_arguments(t_exec *exec_cmd) {
  int i;

  i = 0;
  while (exec_cmd->argv[0] && exec_cmd->argv[0][0] != '\0') {
    i = 0;
    while (exec_cmd->argv[i] && exec_cmd->argv[i + 1]) {
      exec_cmd->argv[i] = exec_cmd->argv[i + 1];
      i++;
    }
    exec_cmd->argv[i] = NULL;
  }
}

char *get_executable_path(char *command, char *path) {
  int i;
  char **path_directories;
  char *command_with_slash;
  char *full_path;

  path_directories = ft_split(path, ':');
  command_with_slash = ft_strjoin("/", command);
  i = 0;
  while (path_directories[i]) {
    full_path = ft_strjoin(path_directories[i], command_with_slash);
    if (access(full_path, X_OK) == 0) {
      // free(command);
      // free_split(path_directories);
      return (full_path);
    }
    free(full_path);
    i++;
  }
  // free(command);
  // free_split(path_directories);
  return (NULL);
}
