/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: archowdh <archowdh@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/12 16:43:56 by archowdh          #+#    #+#             */
/*   Updated: 2026/09/16 20:28:38 by archowdh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	main(int argc, char **argv)
{
	t_cli_params	cli_params;
	t_simulation	simulation;

	if (parse_args(argc, argv, &cli_params))
		return (1);
	simulation.cli_params = cli_params;
	simulation.coders = malloc(sizeof(t_coder) * (cli_params.number_of_coders));
	simulation.dongles = malloc(sizeof(t_dongle)
			* (cli_params.number_of_coders));
	if (!simulation.coders || !simulation.dongles)
	{
		fprintf(stderr, "ERROR: malloc failed\n");
		free(simulation.coders);
		free(simulation.dongles);
		return (1);
	}
	if (pthread_mutex_init(&simulation.mutex, NULL) != 0
		|| pthread_mutex_init(&simulation.log_mutex, NULL) != 0
		|| pthread_cond_init(&simulation.condition, NULL) != 0)
	{
		fprintf(stderr, "ERROR: init failed\n");
		free(simulation.coders);
		free(simulation.dongles);
		return (1);
	}
	if (!create_dongles(&simulation))
	{
		fprintf(stderr, "ERROR: dongle init failed\n");
		free(simulation.coders);
		free(simulation.dongles);
		return (1);
	}
	simulation.simulation_stop = false;
	simulation.next_request_order = 0;
	simulation.start_time = get_time_ms();
	if (create_threads(&simulation) || create_monitor_thread(&simulation))
	{
		free(simulation.coders);
		free(simulation.dongles);
		return (1);
	}
	if (join_threads(&simulation))
	{
		free(simulation.coders);
		return (1);
	}
	join_threads(&simulation);
	pthread_mutex_lock(&simulation.mutex);
	simulation.simulation_stop = true;
	pthread_cond_broadcast(&simulation.condition);
	pthread_mutex_unlock(&simulation.mutex);
	pthread_join(simulation.monitor_thread, NULL);
	destroy_dongles(&simulation);
	pthread_mutex_destroy(&simulation.mutex);
	pthread_mutex_destroy(&simulation.log_mutex);
	pthread_cond_destroy(&simulation.condition);
	free(simulation.coders);
	free(simulation.dongles);
	return (0);
}
