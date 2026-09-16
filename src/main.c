/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: archowdh <archowdh@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/12 16:43:56 by archowdh          #+#    #+#             */
/*   Updated: 2026/09/16 21:19:30 by archowdh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static bool	fail_init(t_simulation *simulation)
{
	free(simulation->coders);
	free(simulation->dongles);
	return (false);
}

static bool	run_simulation(t_simulation *simulation)
{
	if (create_threads(simulation) || create_monitor_thread(simulation))
		return (false);
	join_threads(simulation);
	pthread_mutex_lock(&simulation->mutex);
	simulation->simulation_stop = true;
	pthread_cond_broadcast(&simulation->condition);
	pthread_mutex_unlock(&simulation->mutex);
	pthread_join(simulation->monitor_thread, NULL);
	return (true);
}

static bool	init_simulation(t_simulation *simulation, t_cli_params cli_params)
{
	simulation->cli_params = cli_params;
	simulation->coders = malloc(sizeof(t_coder) * cli_params.number_of_coders);
	simulation->dongles = malloc(sizeof(t_dongle)
			* cli_params.number_of_coders);
	if (!simulation->coders || !simulation->dongles)
		return (fail_init(simulation));
	if (pthread_mutex_init(&simulation->mutex, NULL) != 0
		|| pthread_mutex_init(&simulation->log_mutex, NULL) != 0
		|| pthread_cond_init(&simulation->condition, NULL) != 0)
		return (fail_init(simulation));
	if (!create_dongles(simulation))
		return (fail_init(simulation));
	simulation->simulation_stop = false;
	simulation->next_request_order = 0;
	return (true);
}

static void	cleanup_simulation(t_simulation *simulation)
{
	destroy_dongles(simulation);
	pthread_mutex_destroy(&simulation->mutex);
	pthread_mutex_destroy(&simulation->log_mutex);
	pthread_cond_destroy(&simulation->condition);
	free(simulation->coders);
	free(simulation->dongles);
}

int	main(int argc, char **argv)
{
	t_cli_params	cli_params;
	t_simulation	simulation;

	if (!parse_args(argc, argv, &cli_params))
		return (1);
	if (!init_simulation(&simulation, cli_params))
	{
		fprintf(stderr, "ERROR: initialization failed\n");
		return (1);
	}
	simulation.start_time = get_time_ms();
	if (!run_simulation(&simulation))
	{
		cleanup_simulation(&simulation);
		return (1);
	}
	cleanup_simulation(&simulation);
	return (0);
}
