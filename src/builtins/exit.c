/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msennane <msennane@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/05 16:03:46 by msennane          #+#    #+#             */
/*   Updated: 2024/12/05 17:03:09 by msennane         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	ft_exit(char **argv, t_shell_context *context)
{
	long long	exit_code;
	int			over_under_flow;

	if (argv[1])
	{
		if (!is_numeric(argv[1]))
			non_num_arg(argv[1], context);
		else if (argv[2])
			too_many_args(context);
		else
		{
			ft_putstr_fd("exit\n", STDOUT_FILENO);
			exit_code = ft_atoll(argv[1], &over_under_flow);
			if (over_under_flow)
			{
				ft_putstr_fd("voidshell: exit: ", STDERR_FILENO);
				ft_putstr_fd(argv[1], STDERR_FILENO);
				ft_putstr_fd(": numeric argument required\n", STDERR_FILENO);
				terminate_cleanly(context, 255);
			}
			terminate_cleanly(context, exit_code);
		}
	}
}
