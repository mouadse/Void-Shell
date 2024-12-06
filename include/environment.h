/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   environment.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msennane <msennane@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/30 00:25:14 by msennane          #+#    #+#             */
/*   Updated: 2024/12/06 12:56:35 by msennane         ###   ########.fr       */
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
void					init_env_var(t_env_var **env_var_list, char **envp);
void					extract_and_push(t_env_var **env_var_list,
							char *env_var);
char					**envp_to_env_vector(t_env_var *env_vars);
void	update_shell_lvl(t_env_var **env_var_list);

#endif
