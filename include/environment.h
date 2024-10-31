/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   environment.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msennane <msennane@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/30 00:25:14 by msennane          #+#    #+#             */
/*   Updated: 2024/10/31 21:00:32 by msennane         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ENVIRONMENT_H
# define ENVIRONMENT_H

typedef struct s_env_var
{
	char				*key;
	char				*value;
	struct s_env_var	*next;
}						t_env_var;

// functions signatures here

char					*get_env_value(char *key, t_env_var *env);
void					free_env_node(t_env_var *env);
void					free_env(t_env_var *env);
t_env_var				*create_env_var(char *key, char *value);
int						update_env_var(t_env_var *env_var_list,
							t_env_var *new_nod);
void					insert_env_var(t_env_var **env_var_list,
							t_env_var *new_nod);

#endif
