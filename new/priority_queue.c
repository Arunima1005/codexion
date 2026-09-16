#include "codexion.h"

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

static void	swap_coders(t_coder **a, t_coder **b)
{
	t_coder	*tmp;

	tmp = *a;
	*a = *b;
	*b = tmp;
}

static void	heapify_up(t_priority_queue *queue, int index)
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

static void	heapify_down(t_priority_queue *queue, int index)
{
	int	left;
	int	right;
	int	best;

	while (true)
	{
		left = 2 * index + 1;
		right = 2 * index + 2;
		best = index;
		if (left < queue->size
			&& has_higher_priority(queue->coders[left], queue->coders[best]))
			best = left;
		if (right < queue->size
			&& has_higher_priority(queue->coders[right], queue->coders[best]))
			best = right;
		if (best == index)
			break ;
		swap_coders(&queue->coders[index], &queue->coders[best]);
		index = best;
	}
}

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