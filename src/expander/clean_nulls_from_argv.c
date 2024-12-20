/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   clean_nulls_from_argv.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msennane <msennane@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/21 00:47:39 by msennane          #+#    #+#             */
/*   Updated: 2024/12/21 00:52:56 by msennane         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

// void	clean_nulls_from_argv(char **argv, int size)
// {
// 	int		i;
// 	int		j;
// 	size_t	len;

// 	i = 0;
// 	j = 0;
// 	while (i < size)
// 	{
// 		if (argv[i] != NULL)
// 		{
// 			if (argv[i][0] == '\x01' && argv[i][1] == '\0')
// 			{
// 				i++;
// 				continue ;
// 			}
// 			if (argv[i][0] == '\x01')
// 				ft_memmove(argv[i], argv[i] + 1, ft_strlen(argv[i]));
// 			len = ft_strlen(argv[i]);
// 			while (len > 0 && argv[i][len - 1] == '\x01')
// 			{
// 				argv[i][len - 1] = '\0';
// 				len--;
// 			}
// 			argv[j] = argv[i];
// 			j++;
// 		}
// 		i++;
// 	}
// 	while (j < size)
// 	{
// 		argv[j] = NULL;
// 		j++;
// 	}
// }

static void clean_string(char *str)
{
	size_t len;

	if (!str)
		return;
	// Skip strings that are just '\x01'
	if (str[0] == '\x01' && str[1] == '\0')
		return;
	// Remove leading '\x01'
	if (str[0] == '\x01')
		ft_memmove(str, str + 1, ft_strlen(str));
	// Remove trailing '\x01' characters
	len = ft_strlen(str);
	while (len > 0 && str[len - 1] == '\x01')
	{
		str[len - 1] = '\0';
		len--;
	}
}

void    clean_nulls_from_argv(char **argv, int size)
{
	int     i;
	int     j;

	i = 0;
	j = 0;
	while (i < size)
	{
		if (argv[i] != NULL)
		{
			if (argv[i][0] == '\x01' && argv[i][1] == '\0')
			{
				i++;
				continue;
			}
			clean_string(argv[i]);
			argv[j] = argv[i];
			j++;
		}
		i++;
	}
	while (j < size)
		argv[j++] = NULL;
}
