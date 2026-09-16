#include "codexion.h"

static void	init_coder(t_simulation *simulation, int i, int n)
{
	simulation->coders[i].id = i + 1;
	simulation->coders[i].number_of_compiles = 0;
	simulation->coders[i].last_compile_start_time = simulation->start_time;
	simulation->coders[i].request_order = -1;
	simulation->coders[i].simulation = simulation;
	simulation->coders[i].burnout = false;
	simulation->coders[i].dongles[0] = &simulation->dongles[(i + 1) % n];
	simulation->coders[i].dongles[1] = &simulation->dongles[i];
}

bool	create_threads(t_simulation *simulation)
{
	int	i;
	int	n;

	i = 0;
	n = simulation->cli_params.number_of_coders;
	while (i < n)
	{
		init_coder(simulation, i, n);
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