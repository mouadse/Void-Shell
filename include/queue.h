/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   queue.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msennane <msennane@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/30 00:35:15 by msennane          #+#    #+#             */
/*   Updated: 2024/10/30 00:36:21 by msennane         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef QUEUE_H
# define QUEUE_H

typedef struct s_node
{
	void			*data;
	struct s_node	*next;
}					t_node;

typedef struct s_queue
{
	t_node			*front;
	t_node			*rear;
}					t_queue;

#endif
