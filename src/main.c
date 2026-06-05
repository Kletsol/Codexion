/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbonnet <lbonnet@student.42mulhouse.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/03 15:04:07 by lbonnet           #+#    #+#             */
/*   Updated: 2026/06/05 12:10:30 by lbonnet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	main(int ac, char **av)
{
	t_sim	sim;

	if (ac != 9)
	{
		print_error(ERROR_MISSING_ARG);
		return (1);
	}
	else if (!parser(av, &sim))
		return (1);
	else
	{
		printf("%s", "[Parsing succeeded !]\n\n");
		init_simulation(&sim);
		start_coders(&sim);
		pthread_create(&sim.monitor, NULL, monitor_routine, &sim);
		wait_threads(&sim);
		destroy_simulation(&sim);
		return (0);
	}
}
