/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   environment_utils.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msennane <msennane@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/30 01:51:48 by msennane          #+#    #+#             */
/*   Updated: 2024/12/05 12:30:16 by msennane         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

t_env_var	*create_env_var(char *key, char *value)
{
	t_env_var	*new;

	new = gc_malloc(sizeof(t_env_var));
	if (!new)
		return (NULL);
	new->key = key;
	new->value = value;
	new->next = NULL;
	return (new);
}

char	*get_env_value(char *key, t_env_var *env)
{
	t_env_var	*head;

	head = env;
	while (head != NULL)
	{
		if (ft_strcmp(head->key, key) == 0)
			return (head->value);
		head = head->next;
	}
	return (NULL);
}

void	free_env_node(t_env_var *node)
{
	if (!node)
		return ;
	node->key = NULL;
	node->value = NULL;
	node = NULL;
}

void	free_env(t_env_var *env)
{
	t_env_var	*head;
	t_env_var	*tmp;

	head = env;
	while (head != NULL)
	{
		tmp = head;
		head = head->next;
	}
}
