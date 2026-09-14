/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: archowdh <archowdh@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/12 16:36:39 by archowdh          #+#    #+#             */
/*   Updated: 2026/09/12 18:54:05 by archowdh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CODEXION_H
# define CODEXION_H
# include <pthread.h>
# include <stdio.h>
# include <stdbool.h>
# include <string.h>
# include <stdlib.h>
# include <unistd.h>
# include <sys/time.h>

typedef struct s_simulation t_simulation;
typedef struct s_dongle t_dongle;

typedef enum e_scheduler
{
	FIFO,
	EDF
}	t_scheduler;

typedef struct s_cli_params
{
	int				number_of_coders;
	int				time_to_burnout;
	int				time_to_compile;
	int				time_to_debug;
	int				time_to_refactor;
	int				number_of_compiles_required;
	int				dongle_cooldown;
	t_scheduler		scheduler;
}	t_cli_params;

typedef struct s_coder
{
	int				id;
	int				number_of_compiles;
	long			last_compile_start_time;
	pthread_t		thread;
	t_simulation	*simulation;
	t_dongle		*dongles[2];
}	t_coder;

typedef struct s_simulation
{
	t_cli_params		cli_params;
	t_coder				*coders;
	pthread_mutex_t		mutex;
	pthread_cond_t		condition;
    long				start_time;
	bool				end_time;
	t_dongle			*dongles;
}	t_simulation;

typedef struct s_dongle
{
	bool	is_available;
	int		id;
}	t_dongle;

bool	parse_args(int argc, char **argv, t_cli_params *cli_params);
bool	create_threads(t_simulation *simulation);
void	*work(void *arg);
bool	join_threads(t_simulation *simulation);
bool	store_cli_params(char **argv, t_cli_params *cli_params);
void    get_usleep_ms(long ms);
long 	get_elapsed_time(t_simulation *simulation);
long 	get_time_ms(void);
void 	create_dongles(t_simulation *simulation);
void 	release_dongle(t_coder *coder);
bool 	take_dongle(t_coder *coder);
#endif