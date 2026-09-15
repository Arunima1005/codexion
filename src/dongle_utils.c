/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dongle_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: archowdh <archowdh@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/15 10:59:01 by archowdh          #+#    #+#             */
/*   Updated: 2026/09/15 18:40:31 by archowdh         ###   ########.fr       */
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
		simulation->dongles[i].is_available = 1;
        simulation->dongles[i].available_time = 0;
		i++;
	}
}

bool	take_dongle(t_coder *coder)
{
    struct timespec	timeout;
	long			current_time;

    pthread_mutex_lock(&coder->simulation->mutex);

	while (!coder->simulation->simulation_stop)
	{
		current_time = get_time_ms();

		if (coder->dongles[0]->is_available
			&& coder->dongles[1]->is_available
			&& coder->dongles[0]->available_time <= current_time
			&& coder->dongles[1]->available_time <= current_time)
		{
			coder->dongles[0]->is_available = 0;
			coder->dongles[1]->is_available = 0;

			pthread_mutex_unlock(&coder->simulation->mutex);
			return (true);
		}

		/*
		 * Wait until the earliest relevant cooldown expires.
		 */
		if (coder->dongles[0]->available_time
			> coder->dongles[1]->available_time)
			get_timespec(coder->dongles[1]->available_time, &timeout);
		else
			get_timespec(coder->dongles[0]->available_time, &timeout);

		pthread_cond_timedwait(
			&coder->simulation->condition,
			&coder->simulation->mutex,
			&timeout);
	}
	pthread_mutex_unlock(&coder->simulation->mutex);
	return (1);
}

void	release_dongle(t_coder *coder)
{
    long current_time;
    
	pthread_mutex_lock(&coder->simulation->mutex);
	coder->dongles[0]->is_available = 1;
	coder->dongles[1]->is_available = 1;
    current_time = get_time_ms();
    coder->dongles[0]->available_time = current_time + coder->simulation->cli_params.dongle_cooldown;
    coder->dongles[1]->available_time = current_time + coder->simulation->cli_params.dongle_cooldown;
	pthread_cond_broadcast(&coder->simulation->condition);
	pthread_mutex_unlock(&coder->simulation->mutex);
}
