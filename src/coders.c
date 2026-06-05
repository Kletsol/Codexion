/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   coders.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbonnet <lbonnet@student.42mulhouse.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/03 15:04:07 by lbonnet           #+#    #+#             */
/*   Updated: 2026/06/05 11:31:49 by lbonnet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	*coder_routine(void *arg)
{
	int		i;
	t_coder	*coder;

	i = 0;
	coder = (t_coder *)arg;
	while (!simulation_stopped(coder->sim))
	{
		set_state(coder, COMPILING);
		print_status(coder, "is compiling");
		smart_sleep(coder->sim->time_to_compile, coder->sim);
		set_state(coder, DEBUGGING);
		smart_sleep(coder->sim->time_to_debug, coder->sim);
		print_status(coder, "is debugging");
		set_state(coder, REFACTORING);
		print_status(coder, "is refactoring");
		smart_sleep(coder->sim->time_to_refactor, coder->sim);
	}
	return (NULL);
}

bool	start_coders(t_sim *sim)
{
	int	i;

	i = 0;
	while (i < sim->nb_coders)
	{
		if (pthread_create(&sim->coders[i].thread, NULL,
				coder_routine, &sim->coders[i]) != 0)
			return (false);
		i++;
	}
	return (true);
}

void	wait_threads(t_sim *sim)
{
	int	i;

	i = 0;
	while (i < sim->nb_coders)
	{
		pthread_join(sim->coders[i].thread, NULL);
		i++;
	}
	pthread_join(sim->monitor, NULL);
}
