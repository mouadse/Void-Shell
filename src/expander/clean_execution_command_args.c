/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   clean_execution_command_args.c                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msennane <msennane@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/21 00:56:07 by msennane          #+#    #+#             */
/*   Updated: 2024/12/21 00:56:54 by msennane         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"


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
