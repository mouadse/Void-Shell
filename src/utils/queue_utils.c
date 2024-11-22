/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   queue_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msennane <msennane@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/21 18:52:49 by msennane          #+#    #+#             */
/*   Updated: 2024/11/22 15:28:55 by msennane         ###   ########.fr       */
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

	new_node = (t_node *)malloc(sizeof(t_node));
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

	if (!queue->front)
		return (NULL);
	tmp = queue->front;
	data = tmp->data;
	queue->front = queue->front->next;
	if (!queue->front)
		queue->rear = NULL;
	ft_free(tmp);
	return (data);
}

void	*queue_str_convert(t_queue *queue)
{
	// t_node	*tmp;
	// char	*str;
	// int		len;
	// int		i;

	// if (!queue->front)
	// 	return (NULL);
	// len = 0;
	// tmp = queue->front;
	// while (tmp)
	// {
	// 	len++;
	// 	tmp = tmp->next;
	// }
	// str = (char *)malloc(sizeof(char) * (len + 1));
	// tmp = queue->front;
	// i = 0;
	// while (tmp)
	// {
	// 	str[i] = *(char *)tmp->data;
	// 	tmp = tmp->next;
	// 	i++;
	// }
	// str[i] = '\0';
	// return (str);
	// This code wont accomplish the task of converting the queue to a string
}

void	free_queue(t_queue *queue)
{
	t_node	*tmp;

	while (queue->front)
	{
		tmp = queue->front;
		queue->front = queue->front->next;
		ft_free(tmp->data); // to be reviewed
		ft_free(tmp);
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

	new_node = (t_node_char *)malloc(sizeof(t_node_char));
	if (!new_node)
		exit(EXIT_FAILURE);
	new_node->data = data;
	new_node->next = NULL;
	if (queue->rear)
		queue->rear->next = new_node;
	queue->rear = new_node;
	if (!queue->front)
		queue->front = queue->rear;
}

void	enqueue_str(t_queue_char *queue, char *str)
{
	while (*str)
	{
		enqueue_char(queue, *str);
		str++;
	}
}

char	dequeue_char(t_queue_char *queue)
{
	t_node_char	*tmp;
	char		data;

	if (!queue->front)
		return ('\0');
	tmp = queue->front;
	data = tmp->data;
	queue->front = queue->front->next;
	if (!queue->front)
		queue->rear = NULL;
	ft_free(tmp);
	return (data);
}

char	*queue_char_str_convert(t_queue_char *queue)
{
	t_node_char	*tmp;
	char		*str;
	int			len;
	int			i;

	if (!queue->front)
		return (NULL);
	len = 0;
	tmp = queue->front;
	while (tmp)
	{
		len++;
		tmp = tmp->next;
	}
	str = (char *)malloc(sizeof(char) * (len + 1));
	if (!str)
		exit(EXIT_FAILURE);
	tmp = queue->front;
	i = 0;
	while (tmp)
	{
		str[i] = tmp->data;
		tmp = tmp->next;
		i++;
	}
	str[i] = '\0';
	return (str);
}

void	free_queue_char(t_queue_char *queue)
{
	t_node_char	*tmp;

	while (queue->front)
	{
		tmp = queue->front;
		queue->front = queue->front->next;
		ft_free(tmp);
	}
	queue->rear = NULL;
}
