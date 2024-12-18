/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   environment.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msennane <msennane@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/30 01:51:48 by msennane          #+#    #+#             */
/*   Updated: 2024/12/06 12:56:30 by msennane         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

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
				return (1);
			new_value = ft_strdup(new_nod->value);
			if (!new_value)
				return (1);
			tmp->value = new_value;
			return (1);
		}
		tmp = tmp->next;
	}
	return (0);
}

void	insert_env_var(t_env_var **env_var_list, t_env_var *new_nod)
{
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
	insert_env_var(&((*env_var_list)->next), new_nod);
}

void	extract_and_push(t_env_var **env_var_list, char *env_var)
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
			return ;
		new_nod = create_env_var(key, NULL);
		if (!new_nod)
			return ;
	}
	else
	{
		key = ft_substr(env_var, 0, equal - env_var);
		value = ft_strdup(equal + 1);
		if (!key || (equal[1] && !value))
			return ;
		new_nod = create_env_var(key, value);
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
