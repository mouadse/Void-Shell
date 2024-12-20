/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msennane <msennane@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/07 23:39:15 by msennane          #+#    #+#             */
/*   Updated: 2024/12/21 00:48:17 by msennane         ###   ########.fr       */
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
		else if (arg[i + 1] == '%')
		{
			enqueue_char(queue, '$');
			i++;
			while (arg[i] == '%')
			{
				enqueue_char(queue, '%');
				i++;
			}
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

void	clean_execution_command_args(t_command *cmd, t_shell_context *context,
		int *exit_status)
{
	int		i;
	int		size;
	t_exec	*exec;
	char	*res;
	char	**vector;

	if (!cmd || !context)
		return ;
	exec = (t_exec *)cmd;
	i = 0;
	size = 0;
	if (!exec->argv[0])
		return ;
	while (exec->argv[size])
		size++;
	while (exec->argv[i])
	{
		if (has_special_characters(exec->argv[i]))
			exec->argv[i] = clean_argument(exec->argv[i], context, exit_status);
		else
		{
			enqueue(&context->queue, exec->argv[i]);
			enqueue(&context->queue, " ");
		}
		i++;
	}
	clean_nulls_from_argv(exec->argv, size);
	res = queue_str_convert(&context->queue);
	if (!res)
		return ;
	vector = ft_split_beta(res, ' ');
	if (!vector)
		return ;
	clean_nulls_from_argv(vector, size);
	i = 0;
	while (vector[i])
	{
		vector[i] = remove_quotes(vector[i]);
		i++;
	}
	restore_quotes(vector);
	i = 0;
	while (vector[i])
	{
		exec->argv[i] = vector[i];
		i++;
	}
	exec->argv[i] = NULL;
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
