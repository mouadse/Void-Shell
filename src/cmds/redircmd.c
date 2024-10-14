#include "../../include/command.h"

t_redir_data create_redirdata(char *file, char *efile, int fd, int mode) {
  t_redir_data data;

  data.file = file;
  data.efile = efile;
  data.fd = fd;
  data.mode = mode;
  return (data);
}

t_command *create_redircmd(t_command *sub_cmd, t_redir_data data,
                           char redir_type) {
  t_redir *redir_cmd = malloc(sizeof(t_redir));
  if (!redir_cmd)
    return (NULL);
  redir_cmd->subcmd = sub_cmd;
  redir_cmd->type = REDIR;
  redir_cmd->redir_type = redir_type;
  redir_cmd->file = data.file;
  redir_cmd->efile = data.efile;
  redir_cmd->fd = data.fd;
  redir_cmd->mode = data.mode;
  return ((t_command *)redir_cmd);
}
