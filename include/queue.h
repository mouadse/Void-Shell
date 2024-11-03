/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   queue.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msennane <msennane@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/30 00:35:15 by msennane          #+#    #+#             */
/*   Updated: 2024/11/03 16:46:16 by msennane         ###   ########.fr       */
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

void init_queue(t_queue *queue);
void enqueue(t_queue *queue, void *data);
void enqueue_str(t_queue *queue, char *data);
void *dequeue(t_queue *queue);
void *queue_str_convert(t_queue *queue);
void free_queue(t_queue *queue);

#endif
