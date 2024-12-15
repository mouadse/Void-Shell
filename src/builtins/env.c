/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msennane <msennane@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/05 16:03:46 by msennane          #+#    #+#             */
/*   Updated: 2024/12/16 00:05:10 by msennane         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static void	print_env_error(char *arg)
{
	ft_putstr_fd("env: ", STDERR_FILENO);
	ft_putstr_fd(arg, STDERR_FILENO);
	ft_putstr_fd(": No such file or directory\n", STDERR_FILENO);
}

void	env(char **args, t_shell_context *context)
{
	t_env_var	*curr;

	if (args[1])
	{
		curr = context->env_vars;
		while (curr)
		{
			if (ft_strcmp(curr->key, args[1]) == 0)
			{
				if (curr->value)
					printf("%s\n", curr->value);
				terminate_cleanly(context, 0);
			}
			curr = curr->next;
		}
		print_env_error(args[1]);
		terminate_cleanly(context, 127);
	}
	curr = context->env_vars;
	while (curr)
	{
		if (curr->value)
			printf("%s=%s\n", curr->key, curr->value);
		curr = curr->next;
	}
}
