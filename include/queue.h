/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   queue.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msennane <msennane@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/29 21:04:52 by msennane          #+#    #+#             */
/*   Updated: 2024/09/29 21:05:32 by msennane         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef QUEUE_H
# define QUEUE_H

typedef struct s_qnode
{
	void			*data;
	struct s_qnode	*next;
}					t_qnode;

typedef struct s_queue
{
	t_qnode			*head;
	t_qnode			*tail;
}					t_queue;

#endif
