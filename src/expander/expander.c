/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msennane <msennane@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/07 23:39:15 by msennane          #+#    #+#             */
/*   Updated: 2024/12/05 00:32:00 by msennane         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static void	process_argument(char *arg, t_queue_char *queue, int *exit_status,
		t_shell_context *context)
{
	int	i;
	int	*values[2];
	int	prev_i;

	if (!arg || !queue || !exit_status || !context)
		return ;
	i = 0;
	while (arg[i] != '\0')
	{
		prev_i = i;
		if (arg[i] == '\'')
			handle_single_quotes(arg, &i, queue);
		else if (arg[i] == '\"')
		{
			values[0] = &i;
			values[1] = exit_status;
			handle_double_quotes(arg, values, queue, context);
		}
		else if (arg[i] == '$')
		{
			values[0] = &i;
			values[1] = exit_status;
			handle_dollar_sign(arg, values, queue, context);
		}
		else
			enqueue_char(queue, arg[i++]);
		if (prev_i == i && arg[i] != '\0')
			i++;
	}
}

static char	*clean_argument(char *arg, t_shell_context *context,
		int *exit_status)
{
	t_queue_char	queue;
	char			*cleaned_arg;
	char			*home;

	// to-do : this code can benefit from the some improvements
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
	return (cleaned_arg);
}

static void	clean_execution_command_args(t_command *cmd,
		t_shell_context *context, int *exit_status)
{
	int		i;
	t_exec	*exec;

	exec = (t_exec *)cmd;
	i = 0;
	while (exec->argv[i])
	{
		// skip backslashes
		if (has_special_characters(exec->argv[i]))
			exec->argv[i] = clean_argument(exec->argv[i], context, exit_status);
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
			redir->file = clean_argument(redir->file, context, exit_status);
		process_all_commands(redir->sub_cmd, context, exit_status);
	}
}
