/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   log.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: archowdh <archowdh@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/16 20:24:01 by archowdh          #+#    #+#             */
/*   Updated: 2026/09/19 14:41:42 by archowdh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	log_state(t_coder *coder, const char *state)
{
	const char	*color;

	color = COLOR_CYAN;
	if (!strcmp(state, "is debugging"))
		color = COLOR_YELLOW;
	pthread_mutex_lock(&coder->simulation->log_mutex);
	if (!coder->simulation->log_closed)
	{
		line_begin(coder, get_elapsed_time(coder->simulation), color);
		printf("%s", state);
		if (coder->simulation->cli_params.details)
			printf(" (compiles done: %d)", coder->number_of_compiles);
		line_end(coder);
	}
	pthread_mutex_unlock(&coder->simulation->log_mutex);
}

void	log_compile_start(t_coder *coder)
{
	long	timestamp;

	pthread_mutex_lock(&coder->simulation->log_mutex);
	if (!coder->simulation->log_closed)
	{
		timestamp = get_elapsed_time(coder->simulation);
		print_dongle_line(coder, timestamp, coder->dongles[0]);
		print_dongle_line(coder, timestamp, coder->dongles[1]);
		line_begin(coder, timestamp, COLOR_RED);
		printf("is compiling");
		if (coder->simulation->cli_params.details)
			printf(" (compile #%d)", coder->number_of_compiles + 1);
		line_end(coder);
	}
	pthread_mutex_unlock(&coder->simulation->log_mutex);
}

void	log_burnout(t_coder *coder, int compiles)
{
	pthread_mutex_lock(&coder->simulation->log_mutex);
	line_begin(coder, get_elapsed_time(coder->simulation), COLOR_MAGENTA);
	printf("burned out");
	if (coder->simulation->cli_params.details)
		printf(" (compiles done: %d)", compiles);
	line_end(coder);
	coder->simulation->log_closed = true;
	pthread_mutex_unlock(&coder->simulation->log_mutex);
}
