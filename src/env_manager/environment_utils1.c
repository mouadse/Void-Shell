/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   environment_utils1.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msennane <msennane@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/30 01:51:48 by msennane          #+#    #+#             */
/*   Updated: 2024/12/06 12:55:29 by msennane         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static char	*ft_strjoin3(const char *s1, const char *s2, const char *s3)
{
	char	*result;
	size_t	len1;
	size_t	len2;
	size_t	len3;

	if (!s1 || !s2 || !s3)
		return (NULL);
	len1 = ft_strlen(s1);
	len2 = ft_strlen(s2);
	len3 = ft_strlen(s3);
	result = (char *)gc_malloc(len1 + len2 + len3 + 1);
	if (!result)
		return (NULL);
	ft_memcpy(result, s1, len1);
	ft_memcpy(result + len1, s2, len2);
	ft_memcpy(result + len1 + len2, s3, len3);
	result[len1 + len2 + len3] = '\0';
	return (result);
}

static int	fill_env_vector(char **env_vector, t_env_var *env_vars)
{
	int			i;
	t_env_var	*temp;

	i = 0;
	temp = env_vars;
	while (temp)
	{
		if (temp->key)
		{
			if (temp->value)
				env_vector[i] = ft_strjoin3(temp->key, "=", temp->value);
			else
				env_vector[i] = ft_strdup(temp->key);
			if (!env_vector[i])
				return (-1);
			i++;
		}
		temp = temp->next;
	}
	env_vector[i] = NULL;
	return (0);
}

char	**envp_to_env_vector(t_env_var *env_vars)
{
	int			count;
	t_env_var	*temp;
	char		**env_vector;

	count = 0;
	temp = env_vars;
	while (temp)
	{
		count++;
		temp = temp->next;
	}
	env_vector = gc_malloc(sizeof(char *) * (count + 1));
	if (!env_vector)
		return (NULL);
	if (fill_env_vector(env_vector, env_vars) == -1)
		return (NULL);
	return (env_vector);
}
