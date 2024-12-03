/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   memory_cleanup.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msennane <msennane@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/21 12:29:51 by msennane          #+#    #+#             */
/*   Updated: 2024/12/03 13:24:41 by msennane         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static void	cleanup_resources(t_shell_context *context)
{
	ft_free(context->input);
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
