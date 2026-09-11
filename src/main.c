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
        
        int i;
        i = 0;
        while (i < cli_params.number_of_coders){
            simulation.coders[i].id = i;
            simulation.coders[i].number_of_compiles = 0;
            simulation.coders[i].last_compile_time = 0;
            i++;
        }
        free(simulation.coders);
        return (0);
    }
}