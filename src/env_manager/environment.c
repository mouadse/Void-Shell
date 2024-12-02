/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   environment.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msennane <msennane@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/30 01:51:48 by msennane          #+#    #+#             */
/*   Updated: 2024/12/02 12:34:57 by msennane         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

// SHLVL is a special environment variable that tracks the number of times a shell session has been started.
static void	increment_shell_level(t_env_var *env_var_list);

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
			ft_free(tmp->value);
			tmp->value = new_value;
			free_env_node(new_nod);
			return (1);
		}
		tmp = tmp->next;
	}
	return (0);
}

void	insert_env_var(t_env_var **env_var_list, t_env_var *new_nod)
{
	t_env_var	*curr;
	t_env_var	*prev;

	if (update_env_var(*env_var_list, new_nod))
		return ;
	if (!*env_var_list)
	{
		*env_var_list = new_nod;
		return ;
	}
	if (ft_strcmp(new_nod->key, (*env_var_list)->key) < 0)
	{
		new_nod->next = *env_var_list;
		*env_var_list = new_nod;
		return ;
	}
	curr = *env_var_list;
	prev = NULL;
	while (curr && ft_strcmp(new_nod->key, curr->key) >= 0)
	{
		prev = curr;
		curr = curr->next;
	}
	prev->next = new_nod;
	new_nod->next = curr;
}

static void	extract_and_push(t_env_var **env_var_list, char *env_var)
{
	char		*key;
	char		*value;
	char		*equal;
	t_env_var	*new_nod;

	equal = ft_strchr(env_var, '=');
	if (!equal)
	{
		new_nod = create_env_var(ft_strdup(env_var), NULL);
	}
	else
	{
		key = ft_substr(env_var, 0, equal - env_var);
		value = ft_strdup(equal + 1);
		if (ft_strcmp(key, "OLDPWD") == 0)
		{
			new_nod = create_env_var(key, NULL);
			ft_free(value);
		}
		else
			new_nod = create_env_var(key, value);
	}
	insert_env_var(env_var_list, new_nod);
}

void	init_env_var(t_env_var **env_var_list, char **envp)
{
	int	i;

	i = 0;
	while (envp[i])
	{
		extract_and_push(env_var_list, envp[i]);
		i++;
	}
	// Increment SHLVL after initialization
	increment_shell_level(*env_var_list);
}

static void	increment_shell_level(t_env_var *env_var_list)
{
	t_env_var	*tmp;
	char		*new_value;
	int			shlvl;

	tmp = env_var_list;
	while (tmp)
	{
		if (ft_strcmp(tmp->key, "SHLVL") == 0)
		{
			if (tmp->value)
				shlvl = ft_atoi(tmp->value);
			else
				shlvl = 0;
			shlvl += 1;
			new_value = ft_itoa(shlvl);
			if (!new_value)
				return ; // Handle allocation failure as needed
			ft_free(tmp->value);
			tmp->value = new_value;
			break ;
		}
		tmp = tmp->next;
	}
	// If SHLVL not found, optionally add it
	// Handle leaks here accordingly
	if (!tmp)
	{
		new_value = ft_strdup("1");
		if (!new_value)
			return ; // Handle allocation failure as needed
		tmp = create_env_var("SHLVL", new_value);
		if (tmp)
			insert_env_var(&env_var_list, tmp);
		ft_free(new_value);
	}
}
