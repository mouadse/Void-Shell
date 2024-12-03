/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msennane <msennane@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/30 00:27:00 by msennane          #+#    #+#             */
/*   Updated: 2024/12/02 20:19:56 by msennane         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef COMMAND_H
#define COMMAND_H

#include "environment.h"
#include "queue.h"

#define CMD_EXEC 1
#define CMD_REDIR 2
#define CMD_PIPE 3

#define MAX_ARGS 1024

typedef struct s_command {
  int type;
} t_command;

typedef struct s_exec {
  int type;
  char *argv[MAX_ARGS];
  char *eargv[MAX_ARGS];
} t_exec;

typedef struct s_pipe {
  int type;
  t_command *left;
  t_command *right;
} t_pipe;

typedef struct s_redir {
  int type;
  char redir_type;
  int mode;
  int fd;
  char *file;
  char *efile;
  t_command *sub_cmd;
} t_redir;

typedef struct s_redir_data {
  int fd;
  int mode;
  char *file;
  char *efile;
} t_redir_data;

typedef struct s_shell_context {
  char *input;
  char **envp;
  char *last_cmd;
  t_env_var *env_vars;
  t_command *tree;
  t_queue queue;
} t_shell_context;

t_command *create_execcmd(void);
t_command *create_pipecmd(t_command *left, t_command *right);
t_redir_data create_redirdata(char *file, char *efile, int fd, int mode);
t_command *create_redircmd(t_command *subcmd, t_redir_data data,
                           char redir_type);

#endif
