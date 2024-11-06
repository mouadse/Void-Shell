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

#endif
