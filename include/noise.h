/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   noise.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msennane <msennane@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/01 01:31:57 by msennane          #+#    #+#             */
/*   Updated: 2024/11/24 22:45:57 by msennane         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef NOISE_H
# define NOISE_H

void	signal_handler_general(int signum);
void	signal_handler_input(int signum);
void	signal_handler_heredoc(int signum);
void	setup_signals(void);
void	set_signal_handlers(void);

#endif
