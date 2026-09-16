/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   work.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: archowdh <archowdh@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/12 18:21:14 by archowdh          #+#    #+#             */
/*   Updated: 2026/09/16 20:49:22 by archowdh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static bool	coder_should_stop(t_coder *coder)
{
	bool	stop;

	pthread_mutex_lock(&coder->simulation->mutex);
	stop = coder->simulation->simulation_stop
		|| coder->number_of_compiles
			>= coder->simulation->cli_params.number_of_compiles_required;
	pthread_mutex_unlock(&coder->simulation->mutex);
	return (stop);
}

static bool	simulation_stopped(t_coder *coder)
{
	bool	stop;

	pthread_mutex_lock(&coder->simulation->mutex);
	stop = coder->simulation->simulation_stop;
	pthread_mutex_unlock(&coder->simulation->mutex);
	return (stop);
}

void	*work(void *arg)
{
	t_coder	*coder;

	coder = (t_coder *)arg;
	while (!coder_should_stop(coder))
	{
		if (!compile(coder))
			break ;
		if (simulation_stopped(coder))
			break ;
		debug(coder);
		refactor(coder);
	}
	return (NULL);
}
