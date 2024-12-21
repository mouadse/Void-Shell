/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   clean_execution_command_args.c                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msennane <msennane@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/21 00:56:07 by msennane          #+#    #+#             */
/*   Updated: 2024/12/21 01:00:27 by msennane         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

// void	clean_execution_command_args(t_command *cmd, t_shell_context *context,
// 		int *exit_status)
// {
// 	int		i;
// 	int		size;
// 	t_exec	*exec;
// 	char	*res;
// 	char	**vector;

// 	if (!cmd || !context)
// 		return ;
// 	exec = (t_exec *)cmd;
// 	i = 0;
// 	size = 0;
// 	if (!exec->argv[0])
// 		return ;
// 	while (exec->argv[size])
// 		size++;
// 	while (exec->argv[i])
// 	{
// 		if (has_special_characters(exec->argv[i]))
// 			exec->argv[i] = clean_argument(exec->argv[i], context, exit_status);
// 		else
// 		{
// 			enqueue(&context->queue, exec->argv[i]);
// 			enqueue(&context->queue, " ");
// 		}
// 		i++;
// 	}
// 	clean_nulls_from_argv(exec->argv, size);
// 	res = queue_str_convert(&context->queue);
// 	if (!res)
// 		return ;
// 	vector = ft_split_beta(res, ' ');
// 	if (!vector)
// 		return ;
// 	clean_nulls_from_argv(vector, size);
// 	i = 0;
// 	while (vector[i])
// 	{
// 		vector[i] = remove_quotes(vector[i]);
// 		i++;
// 	}
// 	restore_quotes(vector);
// 	i = 0;
// 	while (vector[i])
// 	{
// 		exec->argv[i] = vector[i];
// 		i++;
// 	}
// 	exec->argv[i] = NULL;
// }

static void	process_arguments(t_exec *exec, t_shell_context *context,
		int *exit_status, int size)
{
	int	i;

	i = 0;
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
}

static char	**convert_queue_to_vector(t_shell_context *context, int size)
{
	char	*res;
	char	**vector;

	res = queue_str_convert(&context->queue);
	if (!res)
		return (NULL);
	vector = ft_split_beta(res, ' ');
	if (!vector)
		return (NULL);
	clean_nulls_from_argv(vector, size);
	return (vector);
}

static void	process_and_copy_vector(char **vector, t_exec *exec)
{
	int	i;

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

void	clean_execution_command_args(t_command *cmd, t_shell_context *context,
		int *exit_status)
{
	int		size;
	t_exec	*exec;
	char	**vector;

	if (!cmd || !context)
		return ;
	exec = (t_exec *)cmd;
	if (!exec->argv[0])
		return ;
	size = 0;
	while (exec->argv[size])
		size++;
	process_arguments(exec, context, exit_status, size);
	vector = convert_queue_to_vector(context, size);
	if (!vector)
		return ;
	process_and_copy_vector(vector, exec);
}
