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
		i++;
	}
}

bool	take_dongle(t_coder *coder)
{
	pthread_mutex_lock(&coder->simulation->mutex);

	while (!coder->simulation->simulation_stop && (!coder->dongles[0]->is_available || !coder->dongles[1]->is_available))
	{
		pthread_cond_wait(&coder->simulation->condition,
			&coder->simulation->mutex);
	}
	if (coder->simulation->simulation_stop)
	{
		pthread_mutex_unlock(&coder->simulation->mutex);
		return (0);
	}
	coder->dongles[0]->is_available = 0;
	coder->dongles[1]->is_available = 0;
	pthread_mutex_unlock(&coder->simulation->mutex);
	return (1);
}

void	release_dongle(t_coder *coder)
{
	pthread_mutex_lock(&coder->simulation->mutex);
	coder->dongles[0]->is_available = 1;
	coder->dongles[1]->is_available = 1;
	pthread_cond_broadcast(&coder->simulation->condition);
	pthread_mutex_unlock(&coder->simulation->mutex);
}
