/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msennane <msennane@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/07 23:39:15 by msennane          #+#    #+#             */
/*   Updated: 2024/12/21 01:09:04 by msennane         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

char	*clean_argument(char *arg, t_shell_context *context,
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
			cleaned_arg = home;
		else
			cleaned_arg = ft_strdup("~");
	}
	else
	{
		process_argument(arg, &queue, exit_status, context);
		cleaned_arg = queue_char_str_convert(&queue);
	}
	enqueue(&context->queue, cleaned_arg);
	enqueue(&context->queue, " ");
	return (cleaned_arg);
}

void	restore_quotes(char **vector)
{
	char	*str;
	int		i;
	int		j;

	if (!vector)
		return ;
	i = 0;
	while (vector[i])
	{
		str = vector[i];
		j = 0;
		while (str[j])
		{
			if (str[j] == '\x1F')
				str[j] = '\'';
			j++;
		}
		i++;
	}
}


void	process_all_commands(t_command *cmd, t_shell_context *context,
		int *exit_status)
{
	t_pipe	*pipe;
	t_redir	*redir;

	if (cmd->type == CMD_EXEC)
	{
		clean_execution_command_args(cmd, context, exit_status);
	}
	else if (cmd->type == CMD_PIPE)
	{
		pipe = (t_pipe *)cmd;
		process_all_commands(pipe->left, context, exit_status);
		process_all_commands(pipe->right, context, exit_status);
	}
	else if (cmd->type == CMD_REDIR)
	{
		redir = (t_redir *)cmd;
		if (redir->redir_type != '%' && has_special_characters(redir->file))
		{
			redir->file = clean_argument2(redir->file, context, exit_status);
		}
		process_all_commands(redir->sub_cmd, context, exit_status);
	}
}

int	count_redirections(t_command *cmd)
{
	t_redir	*redir;
	int		count;

	count = 0;
	if (cmd->type == CMD_REDIR)
	{
		redir = (t_redir *)cmd;
		if (redir->redir_type == '%')
			count++;
		count += count_redirections(redir->sub_cmd);
	}
	return (count);
}
