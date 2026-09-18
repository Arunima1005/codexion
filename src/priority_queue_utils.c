/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   priority_queue_utils.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: archowdh <archowdh@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/18 11:38:48 by archowdh          #+#    #+#             */
/*   Updated: 2026/09/18 11:38:52 by archowdh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static void	swap_coders(t_coder **a, t_coder **b)
{
	t_coder	*tmp;

	tmp = *a;
	*a = *b;
	*b = tmp;
}

void	heapify_up(t_priority_queue *queue, int index)
{
	int	parent;

	while (index > 0)
	{
		parent = (index - 1) / 2;
		if (!has_higher_priority(queue->coders[index], queue->coders[parent]))
			break ;
		swap_coders(&queue->coders[index], &queue->coders[parent]);
		index = parent;
	}
}

void	heapify_down(t_priority_queue *queue, int index)
{
	int	left;
	int	right;
	int	best;

	while (true)
	{
		left = 2 * index + 1;
		right = 2 * index + 2;
		best = index;
		if (left < queue->size && has_higher_priority(queue->coders[left],
				queue->coders[best]))
			best = left;
		if (right < queue->size && has_higher_priority(queue->coders[right],
				queue->coders[best]))
			best = right;
		if (best == index)
			break ;
		swap_coders(&queue->coders[index], &queue->coders[best]);
		index = best;
	}
}

bool	has_higher_priority(t_coder *coder1, t_coder *coder2)
{
	long	deadline1;
	long	deadline2;

	if (coder1->simulation->cli_params.scheduler == FIFO)
		return (coder1->request_order < coder2->request_order);
	deadline1 = coder1->last_compile_start_time
		+ coder1->simulation->cli_params.time_to_burnout;
	deadline2 = coder2->last_compile_start_time
		+ coder2->simulation->cli_params.time_to_burnout;
	if (deadline1 != deadline2)
		return (deadline1 < deadline2);
	return (coder1->request_order < coder2->request_order);
}
