/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dongle_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: archowdh <archowdh@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/15 10:59:01 by archowdh          #+#    #+#             */
/*   Updated: 2026/09/16 20:23:16 by archowdh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	create_dongles(t_simulation *simulation)
{
	int	i;

	i = 0;
	while (i < simulation->cli_params.number_of_coders)
	{
		simulation->dongles[i].id = i;
		simulation->dongles[i].is_available = true;
		simulation->dongles[i].available_time = 0;
		if (!init_priority_queue(&simulation->dongles[i].queue,
				simulation->cli_params.number_of_coders))
			return ;
		i++;
	}
}
void	destroy_dongles(t_simulation *simulation)
{
	int	i;

	i = 0;
	while (i < simulation->cli_params.number_of_coders)
	{
		destroy_priority_queue(&simulation->dongles[i].queue);
		i++;
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
	return (deadline1 < deadline2);
}

bool	add_coder_to_queue(t_dongle *dongle, t_coder *coder)
{
	return (priority_queue_push(&dongle->queue, coder));
}

static bool	dongle_ready_for(t_dongle *d, t_coder *coder, long now)
{
	return (d->is_available && d->available_time <= now && d->queue.size > 0
		&& d->queue.coders[0] == coder);
}

bool	take_dongle(t_coder *coder)
{
	long			current_time;
	long			wake_time;
	struct timespec	timeout;
	t_simulation	*simulation;

	simulation = coder->simulation;
	pthread_mutex_lock(&simulation->mutex);
	coder->request_time = get_time_ms();
	coder->request_order = simulation->next_request_order;
	simulation->next_request_order++;
	if (!add_coder_to_queue(coder->dongles[0], coder)
		|| !add_coder_to_queue(coder->dongles[1], coder))
	{
		pthread_mutex_unlock(&simulation->mutex);
		return (false);
	}
	while (!simulation->simulation_stop)
	{
		current_time = get_time_ms();
		if (coder->dongles[0] != coder->dongles[1]
			&& dongle_ready_for(coder->dongles[0], coder, current_time)
			&& dongle_ready_for(coder->dongles[1], coder, current_time))
		{
			coder->dongles[0]->is_available = false;
			coder->dongles[1]->is_available = false;
			priority_queue_pop(&coder->dongles[0]->queue);
			priority_queue_pop(&coder->dongles[1]->queue);
			pthread_mutex_unlock(&simulation->mutex);
			log_state(coder, "has taken a dongle");
			log_state(coder, "has taken a dongle");
			return (true);
		}
		wake_time = coder->dongles[0]->available_time;
		if (coder->dongles[1]->available_time > wake_time)
			wake_time = coder->dongles[1]->available_time;
		get_timespec(wake_time, &timeout);
		pthread_cond_timedwait(&simulation->condition, &simulation->mutex,
			&timeout);
	}
	priority_queue_remove(&coder->dongles[0]->queue, coder);
	priority_queue_remove(&coder->dongles[1]->queue, coder);
	pthread_mutex_unlock(&simulation->mutex);
	return (false);
}

void	release_dongle(t_coder *coder)
{
	long current_time;

	pthread_mutex_lock(&coder->simulation->mutex);
	coder->dongles[0]->is_available = true;
	coder->dongles[1]->is_available = true;
	current_time = get_time_ms();
	coder->dongles[0]->available_time = current_time
		+ coder->simulation->cli_params.dongle_cooldown;

	coder->dongles[1]->available_time = current_time
		+ coder->simulation->cli_params.dongle_cooldown;
	pthread_cond_broadcast(&coder->simulation->condition);
	pthread_mutex_unlock(&coder->simulation->mutex);
}