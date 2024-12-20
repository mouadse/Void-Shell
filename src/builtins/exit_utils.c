/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msennane <msennane@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/05 16:03:46 by msennane          #+#    #+#             */
/*   Updated: 2024/12/16 02:47:35 by msennane         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static int	check_overflow(long long num, int sign, int digit)
{
	long long	max_div_10;
	long long	min_div_10;

	max_div_10 = LLONG_MAX / 10;
	min_div_10 = LLONG_MIN / 10;
	if (sign == 1)
	{
		if (num > max_div_10 || (num == max_div_10 && digit > LLONG_MAX % 10))
			return (1);
	}
	else
	{
		if (num < min_div_10 || (num == min_div_10 && digit > -(LLONG_MIN
					% 10)))
			return (1);
	}
	return (0);
}

long long	ft_atoll(const char *str, int *over_under_flow)
{
	int			i;
	int			sign;
	long long	num;

	i = 0;
	sign = 1;
	num = 0;
	while (str[i] && ((str[i] >= 9 && str[i] <= 13) || str[i] == 32))
		i++;
	if (str[i] && (str[i] == '+' || str[i] == '-'))
	{
		if (str[i++] == '-')
			sign = -1;
	}
	while (str[i] && (str[i] >= '0' && str[i] <= '9'))
	{
		if (check_overflow(num, sign, str[i] - '0'))
			return (*over_under_flow = 1, 0);
		if (sign == 1)
			num = (num * 10) + (str[i] - '0');
		else
			num = (num * 10) - (str[i] - '0');
		i++;
	}
	return (*over_under_flow = 0, num);
}

void	non_num_arg(char *arg, t_shell_context *context)
{
	ft_putstr_fd("exit\n", STDOUT_FILENO);
	ft_putstr_fd("voidshell: exit: ", STDERR_FILENO);
	ft_putstr_fd(arg, STDERR_FILENO);
	ft_putstr_fd(": numeric argument required\n", STDERR_FILENO);
	terminate_cleanly(context, 2);
}

void	too_many_args(t_shell_context *context)
{
	ft_putstr_fd("exit\n", STDOUT_FILENO);
	ft_putstr_fd("voidshell: exit: too many arguments\n", STDERR_FILENO);
	terminate_cleanly(context, 1);
}
