/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   clean_nulls_from_argv.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msennane <msennane@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/21 00:47:39 by msennane          #+#    #+#             */
/*   Updated: 2024/12/21 00:48:59 by msennane         ###   ########.fr       */
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


// Clean a single string from special characters
static void clean_string(char *str)
{
    size_t len;

    if (!str || (str[0] == '\x01' && str[1] == '\0'))
        return;
    if (str[0] == '\x01')
        ft_memmove(str, str + 1, ft_strlen(str));
    len = ft_strlen(str);
    while (len > 0 && str[len - 1] == '\x01')
    {
        str[len - 1] = '\0';
        len--;
    }
}

// Clean and reorganize the array
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
            clean_string(argv[i]);
            if (argv[i][0] != '\0')  // Only keep non-empty strings
            {
                argv[j] = argv[i];
                j++;
            }
        }
        i++;
    }
    while (j < size)
        argv[j++] = NULL;
}
