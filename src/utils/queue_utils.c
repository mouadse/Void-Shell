/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   queue_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msennane <msennane@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/21 18:52:49 by msennane          #+#    #+#             */
/*   Updated: 2024/12/05 12:49:13 by msennane         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	init_queue(t_queue *queue)
{
	queue->front = NULL;
	queue->rear = NULL;
}

void	enqueue(t_queue *queue, void *data)
{
	t_node	*new_node;

	new_node = (t_node *)gc_malloc(sizeof(t_node));
	if (!new_node)
		return ;
	new_node->data = data;
	new_node->next = NULL;
	if (queue->rear)
		queue->rear->next = new_node;
	queue->rear = new_node;
	if (!queue->front)
		queue->front = queue->rear;
}

void	*dequeue(t_queue *queue)
{
	t_node	*tmp;
	void	*data;

	if (!queue || !queue->front)
		return (NULL);
	tmp = queue->front;
	data = tmp->data;
	queue->front = queue->front->next;
	if (!queue->front)
		queue->rear = NULL;
	return (data);
}

void	*queue_str_convert(t_queue *queue)
{
	int		len;
	char	*str;
	char	*line;
	t_node	*tmp;

	if (!queue || !queue->front)
		return (NULL);
	len = 0;
	tmp = queue->front;
	while (tmp)
	{
		len += ft_strlen(tmp->data);
		tmp = tmp->next;
	}
	str = (char *)ft_calloc(len + 1, sizeof(char));
	while (queue->front)
	{
		line = dequeue(queue);
		ft_strlcat(str, line, len + 1);
	}
	return (str);
}

void	free_queue(t_queue *queue)
{
	t_node	*tmp;

	if (!queue)
		return ;
	while (queue->front)
	{
		tmp = queue->front;
		queue->front = queue->front->next;
	}
	queue->rear = NULL;
}

void	init_queue_char(t_queue_char *queue)
{
	queue->front = NULL;
	queue->rear = NULL;
}

void	enqueue_char(t_queue_char *queue, char data)
{
	t_node_char	*new_node;

	new_node = (t_node_char *)gc_malloc(sizeof(t_node_char));
	if (!new_node)
		return ;
	new_node->data = data;
	new_node->next = NULL;
	if (queue->rear)
		queue->rear->next = new_node;
	queue->rear = new_node;
	if (!queue->front)
		queue->front = queue->rear;
}

char	dequeue_char(t_queue_char *queue)
{
	t_node_char	*tmp;
	char		data;

	if (!queue || !queue->front)
		return ('\0');
	tmp = queue->front;
	data = tmp->data;
	queue->front = queue->front->next;
	if (!queue->front)
		queue->rear = NULL;
	return (data);
}

void	enqueue_str(t_queue_char *queue, char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		enqueue_char(queue, str[i]);
		i++;
	}
}

char	*queue_char_str_convert(t_queue_char *queue)
{
	t_node_char	*tmp;
	char		*str;
	int			i;
	int			len;

	if (!queue || !queue->front)
		return (NULL);
	len = 0;
	tmp = queue->front;
	while (tmp)
	{
		len++;
		tmp = tmp->next;
	}
	str = (char *)gc_malloc(sizeof(char) * (len + 1));
	if (!str)
		return (NULL);
	i = 0;
	while (queue->front)
		str[i++] = dequeue_char(queue);
	str[i] = '\0';
	return (str);
}
