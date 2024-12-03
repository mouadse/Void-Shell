/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   garbage_collector.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msennane <msennane@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/03 15:03:42 by msennane          #+#    #+#             */
/*   Updated: 2024/12/03 15:17:36 by msennane         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// garbage_collector.c
#include "../../include/minishell.h"

t_garbage_collector	g_gc = {0}; // Definition of the global variable

void	*gc_malloc(size_t size)
{
	void		*ptr;
	t_gc_node	*node;

	ptr = malloc(size);
	if (!ptr)
		return (NULL);
	node = malloc(sizeof(t_gc_node));
	if (!node)
	{
		free(ptr);
		return (NULL);
	}
	node->ptr = ptr;
	node->next = g_gc.allocations;
	g_gc.allocations = node;
	return (ptr);
}

void	gc_free_all(void)
{
	t_gc_node	*current;
	t_gc_node	*tmp;

	current = g_gc.allocations;
	while (current)
	{
		free(current->ptr);
		tmp = current;
		current = current->next;
		free(tmp);
	}
	g_gc.allocations = NULL;
}
