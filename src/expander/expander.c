/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msennane <msennane@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/07 23:39:15 by msennane          #+#    #+#             */
/*   Updated: 2024/12/06 01:06:44 by msennane         ###   ########.fr       */
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
		{
			handle_single_quotes(arg, &i, queue);
		}
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
		{
			enqueue_char(queue, arg[i++]);
		}
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

// addition here
void	clean_nulls_from_argv(char **argv, int size)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	while (i < size)
	{
		if (argv[i] != NULL)
		{
			argv[j] = argv[i];
			j++;
		}
		i++;
	}
	while (j < size)
	{
		argv[j] = NULL;
		j++;
	}
}

static void	clean_execution_command_args(t_command *cmd,
		t_shell_context *context, int *exit_status)
{
	int		i;
	int		size;
	t_exec	*exec;

	exec = (t_exec *)cmd;
	i = 0;
	size = 0;
	while (exec->argv[size])
		size++;
	while (exec->argv[i])
	{
		if (has_special_characters(exec->argv[i]))
			exec->argv[i] = clean_argument(exec->argv[i], context, exit_status);
		i++;
	}
	clean_nulls_from_argv(exec->argv, size);
	// FLAG here
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
