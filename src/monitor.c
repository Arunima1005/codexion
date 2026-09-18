/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: archowdh <archowdh@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/18 11:39:00 by archowdh          #+#    #+#             */
/*   Updated: 2026/09/18 20:17:28 by archowdh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static bool	scan_burnout(t_simulation *simulation, int *out_index)
{
	int		i;
	long	elapsed;

	i = 0;
	while (i < simulation->cli_params.number_of_coders)
	{
		if (!simulation->coders[i].burnout)
		{
			elapsed = get_time_ms()
				- simulation->coders[i].last_compile_start_time;
			if (elapsed >= simulation->cli_params.time_to_burnout)
			{
				*out_index = i;
				return (true);
			}
		}
		i++;
	}
	return (false);
}

static bool	monitor_tick(t_simulation *simulation)
{
	int	index;

	pthread_mutex_lock(&simulation->mutex);
	if (simulation->simulation_stop)
	{
		pthread_mutex_unlock(&simulation->mutex);
		return (true);
	}
	if (scan_burnout(simulation, &index))
	{
		simulation->simulation_stop = true;
		simulation->coders[index].burnout = true;
		pthread_cond_broadcast(&simulation->condition);
		pthread_mutex_unlock(&simulation->mutex);
		log_state(&simulation->coders[index], "burned out");
		return (true);
	}
	pthread_mutex_unlock(&simulation->mutex);
	return (false);
}

void	*monitor(void *arg)
{
	t_simulation	*simulation;

	simulation = (t_simulation *)arg;
	while (!monitor_tick(simulation))
		get_usleep_ms(1);
	return (NULL);
}

bool	create_monitor_thread(t_simulation *simulation)
{
	if (pthread_create(&simulation->monitor_thread, NULL, monitor,
			simulation) != 0)
	{
		fprintf(stderr, "ERROR: failed to create monitor\n");
		return (true);
	}
	return (false);
}
