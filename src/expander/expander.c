/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msennane <msennane@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/07 23:39:15 by msennane          #+#    #+#             */
/*   Updated: 2024/11/07 23:39:16 by msennane         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static char *clean_argument(char *arg, t_shell_context *context,
                            int *exit_status) {
  t_queue_char *queue;
  char *cleaned_arg;

  
  
  return (cleaned_arg);
}
void process_all_commands(t_command *cmd, t_shell_context *context,
                          int *exit_status) {
  int type;

  type = cmd->type;
  if (type == CMD_EXEC) {
    clean_execution_command_args(cmd, context, exit_status);
  } else if (type == CMD_PIPE) {
    t_pipe *pipe;
    pipe = (t_pipe *)cmd;
    process_all_commands(pipe->left, context, exit_status);
    process_all_commands(pipe->right, context, exit_status);
  } else if (type == CMD_REDIR) {
    t_redir *redir;
    redir = (t_redir *)cmd;
    if (redir->redir_type == '%' && has_special_characters(redir->file))
      redir->file = clean_argument(redir->file, context, exit_status);
    process_all_commands(redir->sub_cmd, context, exit_status);
  }
}
