/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   noise.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msennane <msennane@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/01 01:31:57 by msennane          #+#    #+#             */
/*   Updated: 2024/12/01 21:08:32 by msennane         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef NOISE_H
# define NOISE_H
# include "command.h"

void	signal_handler_general(int signum);
void	signal_handler_input(int signum);
void	signal_handler_heredoc(int signum);
void	setup_signals(void);
void	set_signal_handler(t_command *tree);

#endif
