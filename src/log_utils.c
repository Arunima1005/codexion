/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   log_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: archowdh <archowdh@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/19 14:35:14 by archowdh          #+#    #+#             */
/*   Updated: 2026/09/19 14:35:41 by archowdh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	line_begin(t_coder *coder, long timestamp, const char *color)
{
	if (coder->simulation->cli_params.color)
		printf("%s", color);
	printf("%ld %d ", timestamp, coder->id);
}

void	line_end(t_coder *coder)
{
	if (coder->simulation->cli_params.color)
		printf("%s", COLOR_RESET);
	printf("\n");
}

void	print_dongle_line(t_coder *coder, long timestamp, t_dongle *dongle)
{
	line_begin(coder, timestamp, COLOR_GREEN);
	printf("has taken a dongle");
	if (coder->simulation->cli_params.details)
		printf(" (dongle #%d)", dongle->id + 1);
	line_end(coder);
}
