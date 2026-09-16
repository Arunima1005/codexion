/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dongle_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: archowdh <archowdh@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/15 10:59:01 by archowdh          #+#    #+#             */
/*   Updated: 2026/09/16 21:03:29 by archowdh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

bool	create_dongles(t_simulation *simulation)
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
			return (false);
		i++;
	}
	return (true);
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

bool	add_coder_to_queue(t_dongle *dongle, t_coder *coder)
{
	return (priority_queue_push(&dongle->queue, coder));
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