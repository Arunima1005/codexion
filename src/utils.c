/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: archowdh <archowdh@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/12 18:21:14 by archowdh          #+#    #+#             */
/*   Updated: 2026/09/15 18:44:51 by archowdh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	compile(t_coder *coder)
{
	if (!take_dongle(coder))
		return ;
	pthread_mutex_lock(&coder->simulation->mutex);
	coder->last_compile_start_time = get_time_ms();
	printf("%ld Coder %d started and has taken dongles %d and %d\n",
	get_elapsed_time(coder->simulation), coder->id, coder->dongles[0]->id, coder->dongles[1]->id);
	pthread_mutex_unlock(&coder->simulation->mutex);
	get_usleep_ms(coder->simulation->cli_params.time_to_compile);
	coder->number_of_compiles++;
	release_dongle(coder);
	printf("%ld Coder %d has released dongles\n",
		get_elapsed_time(coder->simulation), coder->id);
}
void	debug(t_coder *coder)
{
	printf("%ld Coder %d has started debugging\n",
		get_elapsed_time(coder->simulation), coder->id);
	get_usleep_ms(coder->simulation->cli_params.time_to_debug);
	printf("%ld Coder %d has finished debugging\n",
		get_elapsed_time(coder->simulation), coder->id);
}
void	refactor(t_coder *coder)
{
	printf("%ld Coder %d has started refactoring\n",
		get_elapsed_time(coder->simulation), coder->id);
	get_usleep_ms(coder->simulation->cli_params.time_to_refactor);
	printf("%ld Coder %d has finished refactoring\n",
		get_elapsed_time(coder->simulation), coder->id);
}

void	*work(void *arg)
{
	t_coder	*coder;

	coder = (t_coder *)arg;

	while (true)
	{
		pthread_mutex_lock(&coder->simulation->mutex);

		if (coder->simulation->simulation_stop
			|| coder->number_of_compiles
			>= coder->simulation->cli_params.number_of_compiles_required)
		{
			pthread_mutex_unlock(&coder->simulation->mutex);
			break ;
		}

		pthread_mutex_unlock(&coder->simulation->mutex);

		compile(coder);

		pthread_mutex_lock(&coder->simulation->mutex);

		if (coder->simulation->simulation_stop)
		{
			pthread_mutex_unlock(&coder->simulation->mutex);
			break ;
		}

		pthread_mutex_unlock(&coder->simulation->mutex);

		debug(coder);
		refactor(coder);
	}
		return (NULL);
}

void *monitor(void *arg)
{
    t_simulation *simulation = (t_simulation *)arg;
    int i;
    long elapsed;

    printf("Hello i am monitor\n");
    while (true)
    {
		pthread_mutex_lock(&simulation->mutex);

		if (simulation->simulation_stop)
		{
			pthread_mutex_unlock(&simulation->mutex);
			return (NULL);
		}
        i = 0;
        while (i < simulation->cli_params.number_of_coders)
        {
			if (!simulation->coders[i].burnout)
			{
				elapsed = get_time_ms() - simulation->coders[i].last_compile_start_time;
				if (elapsed >= simulation->cli_params.time_to_burnout)
				{
					simulation->simulation_stop = 1;
					simulation->coders[i].burnout = 1;
					printf("%ld coder %d is bournout\n",
					get_elapsed_time(simulation), simulation->coders[i].id);
					pthread_cond_broadcast(&simulation->condition);
					
					pthread_mutex_unlock(&simulation->mutex);

					return (NULL);
				}
			}
			i++;
		}
		pthread_mutex_unlock(&simulation->mutex);
		get_usleep_ms(1000);
    }
    return (NULL);
}


bool create_monitor_thread(t_simulation *simulation)
{
    if (pthread_create(&simulation->monitor_thread, NULL, monitor, simulation) != 0)
    {
        fprintf(stderr, "ERROR: Failed to create monitor\n");
		return (1);
    }
    return (0);
}

bool	create_threads(t_simulation *simulation)
{
	int	i;
	int	coder_number;

	i = 0;
	coder_number = simulation->cli_params.number_of_coders;
	while (i < coder_number)
	{
		simulation->coders[i].id = i;
		simulation->coders[i].number_of_compiles = 0;
		simulation->coders[i].last_compile_start_time = simulation->start_time;
		simulation->coders[i].simulation = simulation;
		simulation->coders[i].burnout = 0;
		simulation->coders[i].dongles[0] = &simulation->dongles[(i + 1)
			% coder_number];
		simulation->coders[i].dongles[1] = &simulation->dongles[i];
		if (pthread_create(&simulation->coders[i].thread, NULL, work,
				&simulation->coders[i]) != 0)
		{
			fprintf(stderr, "ERROR: Failed to create coder %d\n", i);
			return (1);
		}
		i++;
	}
	return (0);
}

bool	join_threads(t_simulation *simulation)
{
	int i;

	i = 0;
	while (i < simulation->cli_params.number_of_coders)
	{
		if (pthread_join(simulation->coders[i].thread, NULL) != 0)
		{
			fprintf(stderr, "Error: failed to join coder %d\n",
				simulation->coders[i].id);
			return (1);
		}
		printf("coder %d finished\n", simulation->coders[i].id);
		i++;
	}
	return (0);
}
