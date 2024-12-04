/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msennane <msennane@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/02 01:47:29 by msennane          #+#    #+#             */
/*   Updated: 2024/12/04 16:50:30 by msennane         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BUILTINS_H
#define BUILTINS_H

// #=====================# echo #=====================#
void echo(char **argv);

// #=====================# env #=====================#
void env(char **argv, t_shell_context *context);

// #=====================# export #=====================#
// ******* export_utils.c ******** //
// char *extract_variable_name(char *arg, char *equal_sign);
int is_valid_variable_name(char *key);
// ******* export.c ******** //
void export_cmd(char **args, t_env_var *env_var_list);
void export_command(char **args, t_env_var **env_var_list, int *exit_status);

// #=====================# unset #=====================#
void unset_env_var(char **args, t_env_var **env_var_list, int *exit_status);

// #=====================# cd #=====================#
void cd(char **argv, int *exit_status, t_env_var **env_var_list);

// #=====================# exit #=====================#
void exit_command(char **argv, t_shell_context *context);

// #=====================# pwd #=====================#
void pwd(int *exit_status);

#endif
