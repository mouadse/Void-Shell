#include "../../include/command.h"

t_command *create_pipecmd(t_command *left, t_command *right) {
  t_pipe *pipe_cmd = malloc(sizeof(t_pipe));
  if (!pipe_cmd)
    return (NULL);
  pipe_cmd->type = PIPE;
  pipe_cmd->left = left;
  pipe_cmd->right = right;
  return ((t_command *)pipe_cmd);
}
