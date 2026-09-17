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
	if (all_coders_finished(coder->simulation))
	{
		coder->simulation->simulation_stop = true;
		pthread_cond_broadcast(&coder->simulation->condition);
	}
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
