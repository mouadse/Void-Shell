#include "../../include/command.h"
#include <linux/limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*Signatures for our functions */
t_command *parsepipe(char **ps, char *es, int *ret);
int peek(char **ps, char *es, char *toks);
t_command *nulterminate(t_command *cmd);
void release_command_resources(t_command *cmd);
int gettoken(char **ps, char *es, char **q, char **eq);
t_command *create_pipecmd(t_command *left, t_command *right);
t_command *create_redircmd(t_command *sub_cmd, t_redir_data data,
                           char redir_type);
t_command *create_redircmd(t_command *sub_cmd, t_redir_data data,
                           char redir_type);
t_redir_data create_redirdata(char *file, char *efile, int fd, int mode);
t_command *parseexec(char **ps, char *es, int *ret);
t_command *create_execcmd(void);

/*Random comments to sep the code and funcs signatures
 */

void panic(const char *message) {
  fprintf(stderr, "panic: %s\n", message); // to be replaced
  exit(EXIT_FAILURE); // Exits the program with a failure code
}

t_command *parsecmd(char *s, int *exit_status) {
  t_command *cmd;
  char *ps;
  char *es;
  int ret;

  ps = s;
  es = ps + strlen(s); // to be changed later with my own strlen
  ret = 0;
  cmd = parsepipe(&ps, es, &ret);
  peek(&ps, es, "");
  if (ret || (ps != es)) {
    if (ret)
      printf("minishell: too many arguments\n");
    // change to stderror func
    else
      printf("minishell: syntax error\n");
    *exit_status = 258;
    release_command_resources(cmd);
    return (NULL);
  }
  nulterminate(cmd);
  return (cmd);
}

t_command *parsepipe(char **ps, char *es, int *ret) {
  t_command *cmd;

  cmd = parseexec(ps, es, ret);
  if (peek(ps, es, "|")) {
    gettoken(ps, es, NULL, NULL);
    cmd = create_pipecmd(cmd, parsepipe(ps, es, ret));
  }
  return (cmd);
}

t_command *parseredirs(t_command *cmd, char **ps, char *es) {
  int tok;
  char *q;
  char *eq;

  if (peek(ps, es, "<>")) {
    tok = gettoken(ps, es, NULL, NULL);
    if (gettoken(ps, es, &q, &eq) != 'a')
      panic("Missing file for redirections!!!"); // Add this func later
    if (tok == '<') {
      cmd = create_redircmd(cmd, create_redirdata(q, eq, 0, O_RDONLY), '<');
    } else if (tok == '>') {
      cmd = create_redircmd(
          cmd, create_redirdata(q, eq, 1, O_WRONLY | O_CREAT | O_TRUNC), '>');
    } else if (tok == '+') {
      cmd = create_redircmd(
          cmd, create_redirdata(q, eq, 1, O_WRONLY | O_CREAT | O_APPEND), '+');
    } else if (tok == '%') {
      cmd = create_redircmd(cmd, create_redirdata(q, eq, 0, O_RDONLY), '%');
    }
  }
  return (cmd);
}

static void parse_next_redir(t_command **cmd, t_exec **exec_cmd, char **ps,
                             char *es) {
  t_command *tmp;

  if ((*cmd) != (t_command *)*exec_cmd) {
    tmp = (*cmd);
    while (((t_redir *)tmp)->subcmd->type == REDIR) {
      tmp = ((t_redir *)tmp)->subcmd;
      ((t_redir *)tmp)->subcmd = parseredirs((t_command *)(*exec_cmd), ps, es);
    }
  } else {
    (*cmd) = parseredirs((*cmd), ps, es);
  }
}

t_command *parseexec(char **ps, char *es, int *ret) {
  char *q, *eq;
  int tok, argc;
  t_command *cmd;
  t_exec *exec_cmd;

  argc = 0;
  cmd = create_execcmd();
  exec_cmd = (t_exec *)cmd;
  cmd = parseredirs(cmd, ps, es);
  while (!peek(ps, es, "|")) {
    if ((tok = gettoken(ps, es, &q, &eq)) == 0)
      break;
    if (tok != 'a')
      panic("Syntax!!!");
    exec_cmd->argv[argc] = q;
    exec_cmd->eargv[argc] = eq;
    argc++;
    if (argc >= ARG_MAX) {
      release_command_resources(cmd);
      return (NULL);
    }
    parse_next_redir(&cmd, &exec_cmd, ps, es);
  }
  exec_cmd->argv[argc] = NULL;
  exec_cmd->eargv[argc] = NULL;
  return (cmd);
}
