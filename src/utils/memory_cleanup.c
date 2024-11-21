/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   memory_cleanup.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msennane <msennane@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/21 12:29:51 by msennane          #+#    #+#             */
/*   Updated: 2024/11/21 16:25:17 by msennane         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static void	cleanup_resources(t_shell_context *context)
{
	if (context->input)
		ft_free(context->input);
	if (context->tree)
	{
		release_command_resources(context->tree);
		context->tree = NULL;
	}
	if (context->env_vars)
	{
		free_env(context->env_vars);
		context->env_vars = NULL;
	}
	if (context->queue)
	{
		free_queue(context->queue); // to be reviewed later
		context->queue = NULL;
	}
}

void	ft_free(void *ptr)
{
	if (ptr)
	{
		free(ptr);
		ptr = NULL;
	}
}

void	terminate_cleanly(t_shell_context *context, int status)
{
	cleanup_resources(context);
	exit(status);
}

void	terminate_with_error(t_shell_context *context, char *message,
		int status)
{
	cleanup_resources(context);
	perror(message);
	exit(status);
}

void	free_array(char **array)
{
	int	i;

	i = 0;
	while (array[i])
	{
		ft_free(array[i]);
		i++;
	}
	ft_free(array);
}
