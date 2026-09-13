/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: archowdh <archowdh@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/12 16:43:56 by archowdh          #+#    #+#             */
/*   Updated: 2026/09/12 18:53:10 by archowdh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "codexion.h"

int main(int argc, char **argv)
{
    t_cli_params cli_params;
    if (parse_args(argc, argv, &cli_params))
        printf("ERROR: please provide a valid input :");
    else
    {
        t_simulation simulation;
        simulation.cli_params = cli_params;
        simulation.coders = malloc(sizeof(t_coder) * (cli_params.number_of_coders));
        simulation.dongles = malloc(sizeof(t_dongle) * (cli_params.number_of_coders));
        if (!simulation.coders || !simulation.dongles)
        {
            fprintf(stderr, "ERROR: malloc failed\n");
            free(simulation.coders);
            free(simulation.dongles);
            return (1);
        }

        if (pthread_mutex_init(&simulation.mutex, NULL) != 0)
        {
            fprintf(stderr, "ERROR: mutex initialization failed\n");
            free(simulation.coders);
            free(simulation.dongles);
            return (1);
        }

        // printf("calling create dongle \n");
        create_dongles(&simulation);
		simulation.start_time = get_time_ms();
        // printf("end create dongle \n");
        if (create_threads(&simulation))
        {
            free(simulation.coders);
            return (1);
        }
        if (join_threads(&simulation))
        {
            free(simulation.coders);
            return (1);
        }
        pthread_mutex_destroy(&simulation.mutex);
        free(simulation.coders);
        free(simulation.dongles);
        return (0);
    }
}