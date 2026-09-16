/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   priority_queue.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: archowdh <archowdh@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/16 17:00:01 by archowdh          #+#    #+#             */
/*   Updated: 2026/09/16 20:35:45 by archowdh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	swap_coders(t_coder **a, t_coder **b)
{
	t_coder	*tmp;

	tmp = *a;
	*a = *b;
	*b = tmp;
}

void	heapify_up(t_priority_queue *queue)
{
	int	index;
	int	parent;

	index = queue->size - 1;
	while (index > 0)
	{
		parent = (index - 1) / 2;
		if (!has_higher_priority(queue->coders[index], queue->coders[parent]))
			break ;
		swap_coders(&queue->coders[index], &queue->coders[parent]);
		index = parent;
	}
}

bool	priority_queue_push(t_priority_queue *queue, t_coder *coder)
{
	if (queue->size >= queue->max_size)
		return (false);
	queue->coders[queue->size] = coder;
	queue->size++;
	heapify_up(queue);
	return (true);
}

bool	init_priority_queue(t_priority_queue *queue, int max_size)
{
	queue->coders = malloc(sizeof(t_coder *) * max_size);
	if (queue->coders == NULL)
		return (false);
	queue->size = 0;
	queue->max_size = max_size;
	return (true);
}

void	heapify_down(t_priority_queue *queue)
{
	int	index;
	int	left;
	int	right;
	int	best_child;

	index = 0;
	while (1)
	{
		left = 2 * index + 1;
		right = 2 * index + 2;
		best_child = index;
		if (left < queue->size && has_higher_priority(queue->coders[left],
				queue->coders[best_child]))
			best_child = left;
		if (right < queue->size && has_higher_priority(queue->coders[right],
				queue->coders[best_child]))
			best_child = right;
		if (best_child == index)
			break ;
		swap_coders(&queue->coders[index], &queue->coders[best_child]);
		index = best_child;
	}
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
		heapify_down(queue);
	}
	return (coder);
}

void	destroy_priority_queue(t_priority_queue *queue)
{
	free(queue->coders);
	queue->coders = NULL;
	queue->size = 0;
	queue->max_size = 0;
}