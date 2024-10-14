#include "../../include/command.h"

void execute_command(t_command *cmd, t_params *params, int *exit_status) {
  int type;
  type = cmd->type;

  if (type == EXEC) {
    // run_exec(cmd, params, exit_status);
  } else if (type == REDIR) {
    // run_redirs(cmd, params, exit_status);
  } else if (type == PIPE) {
    // run_pipe(cmd, params, exit_status);
  }
  exit(*exit_status);
}
