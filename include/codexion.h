/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: archowdh <archowdh@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/12 16:36:39 by archowdh          #+#    #+#             */
/*   Updated: 2026/09/16 21:04:45 by archowdh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CODEXION_H
# define CODEXION_H
# include <pthread.h>
# include <stdbool.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/time.h>
# include <unistd.h>

typedef struct s_simulation	t_simulation;
typedef struct s_dongle		t_dongle;

typedef enum e_scheduler
{
	FIFO,
	EDF
}							t_scheduler;

typedef struct s_cli_params
{
	int						number_of_coders;
	int						time_to_burnout;
	int						time_to_compile;
	int						time_to_debug;
	int						time_to_refactor;
	int						number_of_compiles_required;
	int						dongle_cooldown;
	t_scheduler				scheduler;
}							t_cli_params;

typedef struct s_coder
{
	int						id;
	int						number_of_compiles;
	long					last_compile_start_time;
	long					request_time;
	int						request_order;
	pthread_t				thread;
	t_simulation			*simulation;
	t_dongle				*dongles[2];
	bool					burnout;
}							t_coder;

typedef struct s_simulation
{
	t_cli_params			cli_params;
	t_coder					*coders;
	pthread_mutex_t			mutex;
	pthread_mutex_t			log_mutex;
	pthread_cond_t			condition;
	long					start_time;
	bool					simulation_stop;
	t_dongle				*dongles;
	pthread_t				monitor_thread;
	int						next_request_order;
}							t_simulation;

typedef struct s_priority_queue
{
	t_coder					**coders;
	int						size;
	int						max_size;
}							t_priority_queue;

typedef struct s_dongle
{
	bool					is_available;
	int						id;
	long					available_time;
	t_priority_queue		queue;
}							t_dongle;


/* parse.c */
bool						parse_args(int argc, char **argv,
								t_cli_params *cli_params);
bool						store_cli_params(char **argv,
								t_cli_params *cli_params);

/* thread_setup.c */
bool						create_threads(t_simulation *simulation);
bool						all_coders_finished(t_simulation *simulation);
bool						join_threads(t_simulation *simulation);
void	stop_and_join_coders(t_simulation *simulation, int count);

/* work.c */
void						*work(void *arg);

/* coder_actions.c */
bool						compile(t_coder *coder);
void						debug(t_coder *coder);
void						refactor(t_coder *coder);

/* monitor.c */
void						*monitor(void *arg);
bool						create_monitor_thread(t_simulation *simulation);

/* dongle_utils.c */
bool						create_dongles(t_simulation *simulation);
void						destroy_dongles(t_simulation *simulation);
bool						add_coder_to_queue(t_dongle *dongle,
								t_coder *coder);
void						release_dongle(t_coder *coder);

/* dongle_scheduler.c */
bool						take_dongle(t_coder *coder);

/* priority_queue.c */
bool						init_priority_queue(t_priority_queue *queue,
								int max_size);
bool						priority_queue_push(t_priority_queue *queue,
								t_coder *coder);
t_coder						*priority_queue_pop(t_priority_queue *queue);
void						priority_queue_remove(t_priority_queue *queue,
								t_coder *coder);
void						destroy_priority_queue(t_priority_queue *queue);

/* priority_queue_utils.c */
void						heapify_up(t_priority_queue *queue, int index);
void						heapify_down(t_priority_queue *queue, int index);
bool						has_higher_priority(t_coder *coder1,
								t_coder *coder2);

/* time_utils.c */
long						get_time_ms(void);
long						get_elapsed_time(t_simulation *simulation);
void						get_timespec(long target_ms, struct timespec *ts);
void						get_usleep_ms(long ms);

/* log.c */
void						log_state(t_coder *coder, const char *state);

#endif