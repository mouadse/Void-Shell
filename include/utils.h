/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msennane <msennane@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/21 12:37:28 by msennane          #+#    #+#             */
/*   Updated: 2024/11/21 12:46:41 by msennane         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef UTILS_H
#define UTILS_H

#include "command.h"

// Memory management functions

void ft_free(void *ptr);
void terminate_cleanly(t_shell_context *context, int status);
void terminate_with_error(t_shell_context *context, char *message, int status);



#endif
