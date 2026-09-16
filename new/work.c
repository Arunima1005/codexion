#include "codexion.h"

bool	compile(t_coder *coder)
{
	if (!take_dongle(coder))
		return (false);
	pthread_mutex_lock(&coder->simulation->mutex);
	if (coder->simulation->simulation_stop)
	{
		pthread_mutex_unlock(&coder->simulation->mutex);
		release_dongle(coder);
		return (false);
	}
	coder->last_compile_start_time = get_time_ms();
	pthread_mutex_unlock(&coder->simulation->mutex);
	log_state(coder, "is compiling");
	get_usleep_ms(coder->simulation->cli_params.time_to_compile);
	pthread_mutex_lock(&coder->simulation->mutex);
	coder->number_of_compiles++;
	pthread_mutex_unlock(&coder->simulation->mutex);
	release_dongle(coder);
	return (true);
}

void	debug(t_coder *coder)
{
	log_state(coder, "is debugging");
	get_usleep_ms(coder->simulation->cli_params.time_to_debug);
}

void	refactor(t_coder *coder)
{
	log_state(coder, "is refactoring");
	get_usleep_ms(coder->simulation->cli_params.time_to_refactor);
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
		if (!compile(coder))
			break ;
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

bool	all_coders_finished(t_simulation *simulation)
{
	int	i;

	i = 0;
	while (i < simulation->cli_params.number_of_coders)
	{
		if (simulation->coders[i].number_of_compiles
			< simulation->cli_params.number_of_compiles_required)
			return (false);
		i++;
	}
	return (true);
}

void	*monitor(void *arg)
{
	t_simulation	*simulation;
	int				i;
	long			elapsed;

	simulation = (t_simulation *)arg;
	while (true)
	{
		pthread_mutex_lock(&simulation->mutex);
		if (simulation->simulation_stop)
		{
			pthread_mutex_unlock(&simulation->mutex);
			return (NULL);
		}
		if (all_coders_finished(simulation))
		{
			simulation->simulation_stop = true;
			pthread_cond_broadcast(&simulation->condition);
			pthread_mutex_unlock(&simulation->mutex);
			return (NULL);
		}
		i = 0;
		while (i < simulation->cli_params.number_of_coders)
		{
			if (!simulation->coders[i].burnout)
			{
				elapsed = get_time_ms()
					- simulation->coders[i].last_compile_start_time;
				if (elapsed >= simulation->cli_params.time_to_burnout)
				{
					simulation->simulation_stop = true;
					simulation->coders[i].burnout = true;
					pthread_cond_broadcast(&simulation->condition);
					pthread_mutex_unlock(&simulation->mutex);
					log_state(&simulation->coders[i], "burned out");
					return (NULL);
				}
			}
			i++;
		}
		pthread_mutex_unlock(&simulation->mutex);
		get_usleep_ms(1);
	}
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

bool	create_threads(t_simulation *simulation)
{
	int	i;
	int	n;

	i = 0;
	n = simulation->cli_params.number_of_coders;
	while (i < n)
	{
		simulation->coders[i].id = i + 1;
		simulation->coders[i].number_of_compiles = 0;
		simulation->coders[i].last_compile_start_time = simulation->start_time;
		simulation->coders[i].request_order = -1;
		simulation->coders[i].simulation = simulation;
		simulation->coders[i].burnout = false;
		simulation->coders[i].dongles[0] = &simulation->dongles[(i + 1) % n];
		simulation->coders[i].dongles[1] = &simulation->dongles[i];
		if (pthread_create(&simulation->coders[i].thread, NULL, work,
				&simulation->coders[i]) != 0)
		{
			fprintf(stderr, "ERROR: failed to create coder %d\n", i + 1);
			return (true);
		}
		i++;
	}
	return (false);
}

bool	join_threads(t_simulation *simulation)
{
	int	i;

	i = 0;
	while (i < simulation->cli_params.number_of_coders)
	{
		pthread_join(simulation->coders[i].thread, NULL);
		i++;
	}
	return (false);
}