/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msennane <msennane@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/03 16:39:23 by msennane          #+#    #+#             */
/*   Updated: 2024/11/03 17:07:48 by msennane         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void replace_var_in_line(int *index, char *line, t_queue *q) {
  char *var_name;
  char *var_value;


}

char *process_line(t_shell_context *context, char *line, int *exit_status) {
  
  return (NULL);
}

char *read_heredoc_input(char *del, t_shell_context *context,
                         int *exit_status) {
  char *line;
  char *heredoc_line;
  t_queue queue;
  int tty_fd;

  tty_fd = open("/dev/tty", O_RDWR);
  if (tty_fd < 0) {
    perror("open");
    exit(EXIT_FAILURE); // Replace with custom error handling and memory cleanup
  }
  dup2(tty_fd, STDIN_FILENO);
  close(tty_fd);
  init_queue(&queue);
  while (1) {
    line = readline("> ");
    if (!line) {
      break;
    }
    if (ft_strlen(line) == ft_strlen(del) + 1 &&
        ft_strncmp(line, del, ft_strlen(line) - 1) == 0) {
      free(line);
      break;
    }
    heredoc_line = process_line(context, line, exit_status);
    enqueue(&queue, heredoc_line);
  }
  return (queue_str_convert(&queue));
}

void write_heredoc_file(t_shell_context *context, char *content) {
  int fd;

  fd = open(SHELL_HEREDOC_FILE, O_CREAT | O_WRONLY | O_TRUNC, 0644);
  if (fd < 0) {
    perror("open");
    exit(EXIT_FAILURE); // Replace with custom error handling and memory
                        // cleanup
  }
  if (write(fd, content, ft_strlen(content)) < 0) {
    perror("write");
    exit(EXIT_FAILURE); // Replace with custom error handling and memory
                        // cleanup
  }
  close(fd);
}
