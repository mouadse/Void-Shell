#include "../../include/command.h"

static void	free_command_memory(void *command)
{
	if (command != NULL)
	{
		free(command);
	}
}

void	release_command_resources(t_command *cmd)
{
	t_redir	*rcmd;
	t_pipe	*pcmd;
	t_exec	*ecmd;

	if (!cmd)
		return ;
	if (cmd->type == REDIR)
	{
		rcmd = (t_redir *)cmd;
		release_command_resources(rcmd->subcmd);
		free_command_memory(rcmd);
	}
	else if (cmd->type == PIPE)
	{
		pcmd = (t_pipe *)cmd;
		release_command_resources(pcmd->left);
		release_command_resources(pcmd->right);
		free_command_memory(pcmd);
	}
	else if (cmd->type == EXEC)
	{
		ecmd = (t_exec *)cmd;
		free_command_memory(ecmd);
	}
}
