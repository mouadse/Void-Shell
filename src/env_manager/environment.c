/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   environment.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msennane <msennane@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/30 01:51:48 by msennane          #+#    #+#             */
/*   Updated: 2024/10/31 20:34:49 by msennane         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

t_env_var	*create_env_var(char *key, char *value)
{
	t_env_var	*new;

	new = malloc(sizeof(t_env_var));
	if (!new)
		return (NULL);
	new->key = key;
	new->value = value;
	new->next = NULL;
	return (new);
}

int	update_env_var(t_env_var *env_var_list, t_env_var *new_nod)
{
	t_env_var	*tmp;
	char		*new_value;

	if (!env_var_list || !new_nod)
		return (-1);
	tmp = env_var_list;
	while (tmp)
	{
		if (ft_strcmp(tmp->key, new_nod->key) == 0)
		{
			if (!new_nod->value)
				return (free_env_node(new_nod), 1);
			new_value = ft_strdup(new_nod->value);
			if (!new_value)
				return (free_env_node(new_nod), 1);
			free(tmp->value);
			tmp->value = new_value;
			free_env_node(new_nod);
			return (1);
		}
		tmp = tmp->next;
	}
	return (0);
}
