/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   log.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: archowdh <archowdh@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/16 20:24:01 by archowdh          #+#    #+#             */
/*   Updated: 2026/09/16 20:24:02 by archowdh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	log_state(t_coder *coder, const char *state)
{
	pthread_mutex_lock(&coder->simulation->log_mutex);
	printf("%ld %d %s\n", get_elapsed_time(coder->simulation),
		coder->id, state);
	fflush(stdout);
	pthread_mutex_unlock(&coder->simulation->log_mutex);
}