/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   work.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: archowdh <archowdh@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/12 18:21:14 by archowdh          #+#    #+#             */
/*   Updated: 2026/09/18 18:52:34 by archowdh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static bool	simulation_stopped(t_coder *coder)
{
	bool	stop;

	pthread_mutex_lock(&coder->simulation->mutex);
	stop = coder->simulation->simulation_stop;
	pthread_mutex_unlock(&coder->simulation->mutex);
	return (stop);
}

static bool	coder_has_completed_compiles(t_coder *coder)
{
	bool	done;

	pthread_mutex_lock(&coder->simulation->mutex);
	done = coder->number_of_compiles
		>= coder->simulation->cli_params.number_of_compiles_required;
	pthread_mutex_unlock(&coder->simulation->mutex);
	return (done);
}

void	*work(void *arg)
{
	t_coder	*coder;

	coder = (t_coder *)arg;
	while (!simulation_stopped(coder) && !coder_has_completed_compiles(coder))
	{
		if (!compile(coder))
			break ;
		if (simulation_stopped(coder))
			break ;
		debug(coder);
		if (simulation_stopped(coder))
			break ;
		refactor(coder);
	}
	return (NULL);
}
