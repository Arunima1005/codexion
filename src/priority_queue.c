/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   priority_queue.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: archowdh <archowdh@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/16 17:00:01 by archowdh          #+#    #+#             */
/*   Updated: 2026/09/18 11:38:15 by archowdh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

bool	init_priority_queue(t_priority_queue *queue, int max_size)
{
	queue->coders = malloc(sizeof(t_coder *) * max_size);
	if (!queue->coders)
		return (false);
	queue->size = 0;
	queue->max_size = max_size;
	return (true);
}

bool	priority_queue_push(t_priority_queue *queue, t_coder *coder)
{
	if (queue->size >= queue->max_size)
		return (false);
	queue->coders[queue->size] = coder;
	queue->size++;
	heapify_up(queue, queue->size - 1);
	return (true);
}

t_coder	*priority_queue_pop(t_priority_queue *queue)
{
	t_coder	*coder;

	if (queue->size == 0)
		return (NULL);
	coder = queue->coders[0];
	queue->size--;
	if (queue->size > 0)
	{
		queue->coders[0] = queue->coders[queue->size];
		heapify_down(queue, 0);
	}
	return (coder);
}

void	priority_queue_remove(t_priority_queue *queue, t_coder *coder)
{
	int	i;

	i = 0;
	while (i < queue->size && queue->coders[i] != coder)
		i++;
	if (i == queue->size)
		return ;
	queue->size--;
	queue->coders[i] = queue->coders[queue->size];
	if (i < queue->size)
	{
		heapify_down(queue, i);
		heapify_up(queue, i);
	}
}

void	destroy_priority_queue(t_priority_queue *queue)
{
	free(queue->coders);
	queue->coders = NULL;
	queue->size = 0;
	queue->max_size = 0;
}
