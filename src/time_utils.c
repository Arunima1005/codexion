/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   time_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: archowdh <archowdh@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/15 10:59:19 by archowdh          #+#    #+#             */
/*   Updated: 2026/09/19 16:44:39 by archowdh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

long	get_time_ms(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return ((long)(tv.tv_sec * 1000 + tv.tv_usec / 1000));
}

void	get_usleep_ms(long ms)
{
	if (ms > 0)
		usleep(ms * 1000);
}

long	get_elapsed_time(t_simulation *simulation)
{
	return (get_time_ms() - simulation->start_time);
}
