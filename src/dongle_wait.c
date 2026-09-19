/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dongle_wait.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: archowdh <archowdh@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/19 14:34:53 by archowdh          #+#    #+#             */
/*   Updated: 2026/09/19 18:04:08 by archowdh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static long	cooldown_remaining(t_coder *coder, long now)
{
	t_dongle	*a;
	t_dongle	*b;
	long		wake_time;

	a = coder->dongles[0];
	b = coder->dongles[1];
	if (a == b || !a->is_available || !b->is_available)
		return (0);
	if (a->queue.size == 0 || a->queue.coders[0] != coder)
		return (0);
	if (b->queue.size == 0 || b->queue.coders[0] != coder)
		return (0);
	wake_time = a->available_time;
	if (b->available_time > wake_time)
		wake_time = b->available_time;
	if (wake_time <= now)
		return (0);
	return (wake_time - now);
}

static void	sleep_cooldown(t_simulation *simulation, long ms)
{
	if (ms > 5)
		ms = 5;
	pthread_mutex_unlock(&simulation->mutex);
	get_usleep_ms(ms);
	pthread_mutex_lock(&simulation->mutex);
	pthread_cond_broadcast(&simulation->condition);
}

void	wait_for_turn(t_coder *coder, long now)
{
	long	remaining;

	remaining = cooldown_remaining(coder, now);
	if (remaining > 0)
		sleep_cooldown(coder->simulation, remaining);
	else
		pthread_cond_wait(&coder->simulation->condition,
			&coder->simulation->mutex);
}
