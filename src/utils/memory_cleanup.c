/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   memory_cleanup.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msennane <msennane@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/21 12:29:51 by msennane          #+#    #+#             */
/*   Updated: 2024/12/18 22:38:16 by msennane         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static void	cleanup_resources(t_shell_context *context)
{
	release_command_resources(context->tree);
	free_env(context->env_vars);
	free_queue(&context->queue);
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
	ft_free(context->input);
	gc_free_all();
	exit(status);
}

void	terminate_with_error(t_shell_context *context, char *message,
		int status)
{
	gc_free_all();
	perror(message);
	exit(status);
}

void	free_array(char **array)
{
	int	i;

	i = 0;
	while (array[i])
	{
		i++;
	}
}
