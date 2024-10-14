#include "../../include/command.h"

t_command *nulterminate(t_command *cmd);

static void nulterminate_pipe(t_command *cmd) {
  if (!cmd)
    return;
  t_pipe *pcmd;
  pcmd = (t_pipe *)cmd;
  nulterminate(pcmd->left);
  nulterminate(pcmd->right);
}

static void nulterminate_redir(t_command *cmd) {
  if (!cmd)
    return;
  t_redir *rcmd;
  rcmd = (t_redir *)cmd;
  *rcmd->efile = '\0';
  nulterminate(rcmd->subcmd);
}

static void nulterminate_exec(t_command *cmd) {
  if (!cmd)
    return;
  t_exec *ecmd;
  int i;
  ecmd = (t_exec *)cmd;
  i = 0;
  while (ecmd->argv[i] != NULL) {
    *ecmd->eargv[i] = '\0';
    i++;
  }
}

t_command *nulterminate(t_command *cmd) {
  if (!cmd)
    return (NULL);
  if (cmd->type == PIPE) {
    nulterminate_pipe(cmd);
  } else if (cmd->type == REDIR) {
    nulterminate_redir(cmd);
  } else if (cmd->type == EXEC) {
    nulterminate_exec(cmd);
  }
  return (cmd);
}
