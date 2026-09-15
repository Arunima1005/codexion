/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   time_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: archowdh <archowdh@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/15 10:59:19 by archowdh          #+#    #+#             */
/*   Updated: 2026/09/15 11:02:31 by archowdh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

long	get_time_ms(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return ((long)(tv.tv_sec * 1000 + tv.tv_usec / 1000));
}

void	get_timespec(long target_ms, struct timespec *ts)
{
	ts->tv_sec = target_ms / 1000;
	ts->tv_nsec = (target_ms % 1000) * 1000000;
}

void	get_usleep_ms(long ms)
{
	usleep(ms * 1000);
}

long	get_elapsed_time(t_simulation *simulation)
{
	long	current_time;

	current_time = get_time_ms();

	printf("DEBUG: current=%ld start=%ld elapsed=%ld\n",
		current_time,
		simulation->start_time,
		current_time - simulation->start_time);
	return (get_time_ms() - simulation->start_time);
}
