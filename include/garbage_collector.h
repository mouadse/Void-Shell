/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   garbage_collector.h                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msennane <msennane@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/03 15:03:49 by msennane          #+#    #+#             */
/*   Updated: 2024/12/05 12:59:54 by msennane         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GARBAGE_COLLECTOR_H
# define GARBAGE_COLLECTOR_H

# include <stdlib.h>

typedef struct s_gc_node
{
	void					*ptr;
	struct s_gc_node		*next;
}							t_gc_node;

typedef struct s_garbage_collector
{
	t_gc_node				*allocations;
}							t_garbage_collector;

extern t_garbage_collector	g_gc;

void						*gc_malloc(size_t size);
void						gc_free_all(void);

#endif
