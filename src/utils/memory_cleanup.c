/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   memory_cleanup.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msennane <msennane@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/21 12:29:51 by msennane          #+#    #+#             */
/*   Updated: 2024/11/21 12:50:50 by msennane         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"


static void cleanup_resources(t_shell_context *context)
{
//   if (context->input)
//     ft_free(context->input);
//   if (context->tree)
//     // free_ast(context->tree);
//   if (context->
}


void ft_free(void *ptr) {
	if (ptr) {
		free(ptr);
		ptr = NULL;
	}
}

void terminate_cleanly(t_shell_context *context, int status)
{
	cleanup_resources(context);
	exit(status);
}

void terminate_with_error(t_shell_context *context, char *message, int status)
{
	cleanup_resources(context);
	perror(message);
	exit(status);
}
