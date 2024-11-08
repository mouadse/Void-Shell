/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msennane <msennane@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/06 20:46:40 by msennane          #+#    #+#             */
/*   Updated: 2024/11/06 21:00:16 by msennane         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSER_H
#define PARSER_H

#include "command.h"

t_command *cmd_null_terminate(t_command *cmd);
void	release_command_resources(t_command *cmd);

int check_redirection_syntax(char **ps, char *es, int *exit_status);
int validate_pipe_syntax(char **ps, char *es, int *exit_status);
int check_pipe_usage(char **ps, char *es, int *exit_status);
int check_command_validity(char *line, int *exit_status);
#endif
