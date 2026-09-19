/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dongle_scheduler.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: archowdh <archowdh@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/18 11:39:30 by archowdh          #+#    #+#             */
/*   Updated: 2026/09/19 14:39:03 by archowdh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static bool	register_request(t_coder *coder)
{
	coder->request_time = get_time_ms();
	coder->request_order = coder->simulation->next_request_order++;
	if (!add_coder_to_queue(coder->dongles[0], coder))
		return (false);
	if (coder->dongles[1] != coder->dongles[0]
		&& !add_coder_to_queue(coder->dongles[1], coder))
	{
		priority_queue_remove(&coder->dongles[0]->queue, coder);
		return (false);
	}
	return (true);
}

static bool	try_grant_dongles(t_coder *coder, long now)
{
	t_dongle	*a;
	t_dongle	*b;

	a = coder->dongles[0];
	b = coder->dongles[1];
	if (a == b)
		return (false);
	if (!a->is_available || a->available_time > now)
		return (false);
	if (!b->is_available || b->available_time > now)
		return (false);
	if (a->queue.size == 0 || a->queue.coders[0] != coder)
		return (false);
	if (b->queue.size == 0 || b->queue.coders[0] != coder)
		return (false);
	a->is_available = false;
	b->is_available = false;
	priority_queue_pop(&a->queue);
	priority_queue_pop(&b->queue);
	return (true);
}

bool	take_dongle(t_coder *coder)
{
	t_simulation	*simulation;
	long			now;

	simulation = coder->simulation;
	pthread_mutex_lock(&simulation->mutex);
	if (!register_request(coder))
	{
		pthread_mutex_unlock(&simulation->mutex);
		return (false);
	}
	while (!simulation->simulation_stop)
	{
		now = get_time_ms();
		if (try_grant_dongles(coder, now))
		{
			pthread_mutex_unlock(&simulation->mutex);
			return (true);
		}
		wait_for_turn(coder, now);
	}
	return (abort_request(coder));
}
