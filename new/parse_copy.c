#include "codexion.h"

static bool	is_valid_number(const char *str)
{
	int	i;

	if (str[0] == '\0')
		return (false);
	i = 0;
	while (str[i])
	{
		if (str[i] < '0' || str[i] > '9')
			return (false);
		i++;
	}
	return (true);
}

bool	store_cli_params(char **argv, t_cli_params *cli_params)
{
	cli_params->number_of_coders = atoi(argv[1]);
	cli_params->time_to_burnout = atoi(argv[2]);
	cli_params->time_to_compile = atoi(argv[3]);
	cli_params->time_to_debug = atoi(argv[4]);
	cli_params->time_to_refactor = atoi(argv[5]);
	cli_params->number_of_compiles_required = atoi(argv[6]);
	cli_params->dongle_cooldown = atoi(argv[7]);
	if (!strcmp(argv[8], "edf"))
		cli_params->scheduler = EDF;
	else if (!strcmp(argv[8], "fifo"))
		cli_params->scheduler = FIFO;
	else
	{
		fprintf(stderr, "Error: invalid scheduler %s\n", argv[8]);
		return (false);
	}
	return (true);
}

bool	parse_args(int argc, char **argv, t_cli_params *cli_params)
{
	int	i;

	if (argc != 9)
	{
		fprintf(stderr, "Error: expected 8 arguments, got %d\n", argc - 1);
		return (false);
	}
	i = 1;
	while (i < 8)
	{
		if (!is_valid_number(argv[i]))
		{
			fprintf(stderr, "Error: invalid argument %d: %s\n", i, argv[i]);
			return (false);
		}
		i++;
	}
	if (atoi(argv[1]) < 1)
	{
		fprintf(stderr, "Error: number_of_coders must be at least 1\n");
		return (false);
	}
	return (store_cli_params(argv, cli_params));
}