/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   environment.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msennane <msennane@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/30 01:51:48 by msennane          #+#    #+#             */
/*   Updated: 2024/12/04 00:23:10 by msennane         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void		update_shell_lvl(t_env_var **env_var_list);

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
			{
				// Free here and return (free_env_node(new_nod));
				return (1);
			}
			new_value = ft_strdup(new_nod->value);
			if (!new_value)
			{
				// Free here and return (free_env_node(new_nod));
				return (1);
			}
			// ft_free(tmp->value);
			tmp->value = new_value;
			// Transfer ownership; free here
			// free_env_node(new_nod);
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
	if (prev == NULL)
	{
		new_nod->next = curr;
		*env_var_list = new_nod;
	}
	else
	{
		prev->next = new_nod;
		new_nod->next = curr;
	}
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
		key = ft_strdup(env_var);
		if (!key)
			return ; // Avoid processing further
		new_nod = create_env_var(key, NULL);
		if (!new_nod)
		{
			// ft_free(key);
			return ;
		}
	}
	else
	{
		key = ft_substr(env_var, 0, equal - env_var);
		value = ft_strdup(equal + 1);
		if (!key || (equal[1] && !value))
		{
			// ft_free(key);
			// ft_free(value);
			return ;
		}
		if (ft_strcmp(key, "OLDPWD") == 0)
		{
			new_nod = create_env_var(key, NULL);
			// ft_free(value);
		}
		else
		{
			new_nod = create_env_var(key, value);
		}
		/*else if (ft_strcmp(key, "_") == 0)
		{
			new_nod = create_env_var(key, "/bin/bash");
			// ft_free(value);
		} */
	}
	if (new_nod)
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
	update_shell_lvl(env_var_list);
}

void	update_shell_lvl(t_env_var **env_var_list)
{
	char	*shell_lvl;
	char	*new_shell_lvl;
	int		shell_lvl_int;

	shell_lvl = getenv("SHLVL");
	if (!shell_lvl)
	{
		shell_lvl_int = 1;
	}
	else
	{
		shell_lvl_int = ft_atoi(shell_lvl);
		shell_lvl_int++;
	}
	new_shell_lvl = ft_itoa(shell_lvl_int);
	if (!new_shell_lvl)
		return ;
	insert_env_var(env_var_list, create_env_var(ft_strdup("SHLVL"),
			new_shell_lvl));
}


