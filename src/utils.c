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

void	*work(void *arg)
{
	t_coder	*coder;

	coder = (t_coder *)arg;

	printf("Coder %d started\n", coder->id);
	return (NULL);
}

bool create_threads(t_coder *coder, int number_of_coders)
{
    int i;
    i = 0;
    while (i < number_of_coders){
        coder[i].id = i;
        coder[i].number_of_compiles = 0;
        coder[i].last_compile_time = 0;
        
        if (pthread_create(&coder[i].thread, NULL, work, &coder[i]) != 0)
        {    
            fprintf(stderr, "ERROR: Failed to create coder %d\n", i);
            return (1);
        }
        i++;
    }
    return (0);
}

bool	join_threads(t_coder *coders, int number_of_coders)
{
	int	i;

	i = 0;
	while (i < number_of_coders)
	{
		if (pthread_join(coders[i].thread, NULL) != 0)
		{
			fprintf(stderr, "Error: failed to join coder %d\n",
				coders[i].id);
			return (1);
		}
		i++;
	}
	return (0);
}