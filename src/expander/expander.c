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

static char	*clean_argument(char *arg, t_shell_context *context,
		int *exit_status)
{
	t_queue_char	queue;
	char			*cleaned_arg;
	char			*home;

	init_queue_char(&queue);
	if (ft_strcmp(arg, "~") == 0)
	{
		home = get_env_value("HOME", context->env_vars);
		if (home)
		{
			cleaned_arg = home;
		}
		else
		{
			cleaned_arg = ft_strdup("~");// this is added to be fixing the leaks in case of no env : env -i ./minishell
		}
		free_queue_char(&queue); // this is added to be fixing the leaks
	}
	else
	{
		process_argument(arg, &queue, exit_status, context);
		cleaned_arg = queue_char_str_convert(&queue);
	}
	enqueue(context->queue, cleaned_arg);
	return (cleaned_arg);
}
void	process_all_commands(t_command *cmd, t_shell_context *context,
		int *exit_status)
{
	int	type;
		t_pipe *pipe;
		t_redir *redir;

	type = cmd->type;
	if (type == CMD_EXEC)
	{
		clean_execution_command_args(cmd, context, exit_status);
	}
	else if (type == CMD_PIPE)
	{
		pipe = (t_pipe *)cmd;
		process_all_commands(pipe->left, context, exit_status);
		process_all_commands(pipe->right, context, exit_status);
	}
	else if (type == CMD_REDIR)
	{
		redir = (t_redir *)cmd;
		if (redir->redir_type == '%' && has_special_characters(redir->file))
			redir->file = clean_argument(redir->file, context, exit_status);
		process_all_commands(redir->sub_cmd, context, exit_status);
	}
}
