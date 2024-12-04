/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msennane <msennane@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/02 01:47:29 by msennane          #+#    #+#             */
/*   Updated: 2024/12/04 20:15:33 by msennane         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BUILTINS_H
# define BUILTINS_H

// #=====================# echo #=====================#
void	echo(char **argv);

// #=====================# env #=====================#
void	env(char **argv, t_shell_context *context);

// #=====================# export #=====================#
// ******* export_utils.c ******** //
// char *extract_variable_name(char *arg, char *equal_sign);
int		is_valid_var(char *key);
char	*extract_key(char *str, char *equal_pos);
void	insert_exported_var(char *equal_sign, char *key,
			t_env_var **env_var_list);
void	display_env_variables(t_env_var *env_var_list);
// ******* export.c ******** //
void	simple_export(char **args, t_env_var *env_var_list);
void	ft_export(char **args, t_env_var **env_var_list, int *exit_status);

// #=====================# unset #=====================#
void	unset_env(char **args, t_env_var **env_var_list, int *exit_status);

// #=====================# cd #=====================#
void	cd(char **argv, int *exit_status, t_env_var **env_var_list);

// #=====================# exit #=====================#
void	exit_command(char **argv, t_shell_context *context);

// #=====================# pwd #=====================#
void	pwd(int *exit_status);

#endif
