#include "../../include/command.h"
#include <string.h>

t_command *create_execcmd(void) {
  t_exec *exec_cmd = malloc(sizeof(t_exec));
  if (!exec_cmd)
    return (NULL);
  memset(exec_cmd, 0, sizeof(t_exec)); // to be changed later with my own func
  exec_cmd->type = EXEC;
  return ((t_command *)exec_cmd);
}
