/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   initialize.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbonnet <lbonnet@student.42mulhouse.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/03 15:04:07 by lbonnet           #+#    #+#             */
/*   Updated: 2026/06/23 16:08:30 by lbonnet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

bool	init_dongles(t_sim *sim)
{
	int	i;

	i = 0;
	while (i < sim->nb_coders)
	{
		if (pthread_mutex_init(&sim->dongles[i].dongle, NULL) != 0)
			return (false);
		if (pthread_mutex_init(&sim->dongles[i].heap, NULL) != 0)
			return (false);
		if (pthread_mutex_init(&sim->dongles[i].state, NULL) != 0)
			return (false);
		sim->cleanup.nb_dongles++;
		sim->dongles[i].last_use = 0;
		sim->dongles[i].waiters[0] = NULL;
		sim->dongles[i++].waiters[1] = NULL;
	}
	return (true);
}

bool	init_coders(t_sim *sim)
{
	int	i;

	i = 0;
	while (i < sim->nb_coders)
	{
		sim->coders[i].nb_compiles = 0;
		sim->coders[i].left = &sim->dongles[i];
		if (i + 1 < sim->nb_coders)
			sim->coders[i].right = &sim->dongles[i + 1];
		else
			sim->coders[i].right = &sim->dongles[0];
		sim->coders[i].id = i + 1;
		sim->coders[i].compile_start = get_time_ms(sim);
		sim->coders[i].sim = sim;
		if (pthread_mutex_init(&sim->coders[i].coder_mutex, NULL) != 0)
			return (print_error("Failed to initialize mutex"));
		sim->cleanup.nb_coders++;
		i++;
	}
	return (true);
}

bool	init_sim_mutexes(t_sim *sim)
{
	if (pthread_mutex_init(&sim->stop_mutex, NULL) != 0)
		return (print_error("Failed to initialize mutex"));
	sim->cleanup.stop_mutex = true;
	if (pthread_mutex_init(&sim->print_mutex, NULL) != 0)
		return (print_error("Failed to initialize mutex"));
	sim->cleanup.print_mutex = true;
	if (pthread_mutex_init(&sim->sim_mutex, NULL) != 0)
		return (print_error("Failed to initialize mutex"));
	sim->cleanup.sim_mutex = true;
	return (true);
}

bool	init_simulation(t_sim *sim)
{
	sim->cleanup.nb_dongles = 0;
	sim->cleanup.nb_coders = 0;
	sim->stop = false;
	gettimeofday(&sim->start_time, NULL);
	sim->coders = ft_calloc(sim->nb_coders, sizeof(t_coder));
	if (!sim->coders)
		return (print_error("Allocation failed for coders tab"));
	sim->dongles = ft_calloc(sim->nb_coders, sizeof(t_dongle));
	if (!sim->dongles)
		return (print_error("Allocation failed for dongles tab"));
	if (!init_sim_mutexes(sim))
		return (false);
	if (!init_dongles(sim))
		return (false);
	if (!init_coders(sim))
		return (false);
	return (true);
}
