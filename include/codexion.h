#ifndef CODEXION_H
# define CODEXION_H

# include <pthread.h>
# include <stdio.h>
# include <stdbool.h>
# include <string.h>
#include <stdlib.h>

typedef enum e_scheduler
{
	FIFO,
	EDF
}	t_scheduler;

typedef struct s_cli_params
{
	int			number_of_coders;
    int			time_to_burnout;
    int			time_to_compile;
    int			time_to_debug;
    int 		time_to_refactor;
    int			number_of_compiles_required;
    int			dongle_cooldown;
    t_scheduler	scheduler;
}	t_cli_params;

typedef struct s_coder
{
	int		id;
    int     number_of_compiles;
    int     last_compile_time;
	pthread_t thread;

}	t_coder;

typedef struct s_simulation
{
    t_cli_params cli_params;
    t_coder *coders;
    pthread_mutex_t mutex;
    pthread_cond_t condition;

}   t_simulation;



bool parse_args(int argc, char **argv, t_cli_params *cli_params);

#endif