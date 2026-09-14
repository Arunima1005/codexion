/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: archowdh <archowdh@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/12 18:21:14 by archowdh          #+#    #+#             */
/*   Updated: 2026/09/12 20:42:12 by archowdh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

bool is_valid_number(char *str)
{
	int	i;

	if (str[0] == '\0')
		return (0);

	i = 0;
	while (str[i])
	{
		if (str[i] < '0' || str[i] > '9')
			return (0);
		i++;
	}
	return (1);
}

bool store_cli_params(char **argv, t_cli_params *cli_params)
{
    *cli_params = (t_cli_params) {.number_of_coders = atoi(argv[1]),
        .time_to_burnout = atoi(argv[2]),
        .time_to_compile = atoi(argv[3]),
        .time_to_debug = atoi(argv[4]),
        .time_to_refactor = atoi(argv[5]),
        .number_of_compiles_required = atoi(argv[6]),
        .dongle_cooldown = atoi(argv[7])
    };

    if (!strcmp(argv[8], "edf"))
        cli_params->scheduler = EDF;
    else if (!strcmp(argv[8], "fifo"))
        cli_params->scheduler = FIFO;
    else
    {
        fprintf(stderr, "Error: Invalid scheduler %s\n", argv[8]);
        return (1);
    }
    return (0);
}

bool parse_args(int argc, char **argv, t_cli_params *cli_params)
{
    int i;

    i = 1;
    if (argc != 9)
    {
        fprintf(stderr, "Error: Expected 8 arguments, got %d\n", argc - 1);
        return (1);
    }
    while (i < 8)
    {
        if (!is_valid_number(argv[i]) || atoi(argv[i]) == 0)
        {
            fprintf(stderr, "Error: Invalid argument %d : %s \n", i, argv[i]);
            return (1);
        }
        i++;        
    }
    return (store_cli_params(argv, cli_params));
}

void compile(t_coder *coder)
{
    take_dongle(coder);        
    printf("%ld Coder %d started and has taken dongles %d and %d\n",get_elapsed_time(coder->simulation), coder->id, coder->dongles[0]->id, coder->dongles[1]->id);		
    coder->number_of_compiles++;
    coder->last_compile_start_time = get_time_ms();
    get_usleep_ms(coder->simulation->cli_params.time_to_compile);
    release_dongle(coder);
    printf("%ld Coder %d has released dongles\n", get_elapsed_time(coder->simulation), coder->id); 
}
void debug(t_coder *coder)
{
    printf("%ld Coder %d has started debugging\n",get_elapsed_time(coder->simulation), coder->id);
    get_usleep_ms(coder->simulation->cli_params.time_to_debug);
    printf("%ld Coder %d has finished debugging\n", get_elapsed_time(coder->simulation), coder->id);
    
}
void refactor(t_coder *coder)
{
    printf("%ld Coder %d has started refactoring\n", get_elapsed_time(coder->simulation), coder->id);
    get_usleep_ms(coder->simulation->cli_params.time_to_refactor);
    printf("%ld Coder %d has finished refactoring\n", get_elapsed_time(coder->simulation), coder->id);
}

void	*work(void *arg)
{
	t_coder	*coder;
	
	coder = (t_coder *)arg;
    while (!coder->simulation->end_time  && coder->number_of_compiles < coder->simulation->cli_params.number_of_compiles_required)
    {
        long elapsed;

        elapsed = get_time_ms() - coder->last_compile_start_time;

        if (elapsed >= coder->simulation->cli_params.time_to_burnout)
        {
            // Burnout
            coder->simulation->end_time = 1;
            printf("%ld coder %d is bournout\n", get_elapsed_time(coder->simulation), coder->id);
            return (NULL);
        }

        compile(coder);
        debug(coder);
        refactor(coder);
    }
	return (NULL);
}

bool create_threads(t_simulation *simulation)
{
    int i;
    int coder_number;

    i = 0;
    coder_number = simulation->cli_params.number_of_coders;
    while (i < coder_number){
        simulation->coders[i].id = i;
        simulation->coders[i].number_of_compiles = 0;
        simulation->coders[i].last_compile_start_time = simulation->start_time;
		simulation->coders[i].simulation = simulation;

        simulation->coders[i].dongles[0] =
			&simulation->dongles[
				(i + 1) % coder_number];

		simulation->coders[i].dongles[1] =
			&simulation->dongles[i];
        
        if (pthread_create(&simulation->coders[i].thread, NULL, work, &simulation->coders[i]) != 0)
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
	int	i;

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