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
        if (!simulation.coders)
        {
            fprintf(stderr, "ERROR: malloc failed: simulation coder creatation\n");
        }

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
        
        free(simulation.coders);
        return (0);
    }
}