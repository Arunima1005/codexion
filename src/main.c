#include "codexion.h"

int main(int argc, char **argv)
{
    t_cli_params cli_params;
    if (parse_args(argc, argv, &cli_params))
        printf("ERROR: please provide a valid input :");
    else
    {
        //printf("%d", cli_params.number_of_coders);
    }
}