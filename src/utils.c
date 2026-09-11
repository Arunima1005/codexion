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
        if (!is_valid_number(argv[i]) || atoi(argv[i]) < 0)
        {
            fprintf(stderr, "Error: Invalid argument %d : %s \n", i, argv[i]);
            return (1);
        }
        i++;        
    }

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

// void create_coders(t_coder *coder)
// {
//     for 
// }