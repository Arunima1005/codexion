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

        if (create_threads(simulation.coders,
			cli_params.number_of_coders))
        {
            free(simulation.coders);
            return (1);
        }

        if (join_threads(simulation.coders,
                cli_params.number_of_coders))
        {
            free(simulation.coders);
            return (1);
        }
        
        free(simulation.coders);
        return (0);
    }
}