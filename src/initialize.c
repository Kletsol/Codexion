/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   initialize.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbonnet <lbonnet@student.42mulhouse.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/03 15:04:07 by lbonnet           #+#    #+#             */
/*   Updated: 2026/06/05 09:53:52 by lbonnet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

bool	init_heap(t_dongle *dongle, t_sim *sim)
{
	dongle->waiters.capacity = sim->nb_coders;
	dongle->waiters.size = 0;
	dongle->waiters.data = ft_calloc(sim->nb_coders, sizeof(t_request));
	if (!dongle->waiters.data)
		return (false);
	return (true);
}

bool	init_dongles(t_sim *sim)
{
	int	i;

	sim->dongles = ft_calloc(sim->nb_coders, sizeof(t_dongle));
	if (!sim->dongles)
		return (false);
	i = -1;
	while (++i < sim->nb_coders)
	{
		sim->dongles[i].id = i;
		sim->dongles[i].available = true;
		sim->dongles[i].available_at = 0;
		if ((!init_heap(&sim->dongles[i], sim))
			|| pthread_mutex_init(&sim->dongles[i].mutex, NULL) != 0)
		{
			destroy_dongles(sim, i);
			return (false);
		}
		if (pthread_cond_init(&sim->dongles[i].cond, NULL) != 0)
		{
			pthread_mutex_destroy(&sim->dongles[i].mutex);
			destroy_dongles(sim, i);
			return (false);
		}
		printf("Dongle %d initialized\n", sim->dongles[i].id);
	}
	return (true);
}

bool	init_coders(t_sim *sim)
{
	int	i;

	sim->coders = ft_calloc(sim->nb_coders, sizeof(t_coder));
	if (!sim->coders)
		return (false);
	i = 0;
	while (i < sim->nb_coders)
	{
		sim->coders[i].id = i + 1;
		sim->coders[i].sim = sim;
		sim->coders[i].last_compile_start = sim->start_time;
		sim->coders[i].left_dongle = &sim->dongles[i];
		sim->coders[i].right_dongle = &sim->dongles[(i + 1) % sim->nb_coders];
		if (pthread_mutex_init(&sim->coders[i].state_mutex, NULL) != 0)
		{
			destroy_coders(sim, i);
			return (false);
		}
		printf("Coder %d initialized\n", sim->coders[i].id);
		i++;
	}
	return (true);
}

bool	init_simulation(t_sim *sim)
{
	sim->start_time = get_time_ms();
	sim->stop = false;
	if (pthread_mutex_init(&sim->stop_mutex, NULL) != 0)
		return (false);
	if (pthread_mutex_init(&sim->print_mutex, NULL) != 0)
	{
		pthread_mutex_destroy(&sim->stop_mutex);
		return (false);
	}
	if (!init_dongles(sim))
	{
		destroy_global_mutexes(sim);
		return (false);
	}
	if (!init_coders(sim))
	{
		destroy_dongles(sim, sim->nb_coders);
		destroy_global_mutexes(sim);
		return (false);
	}
	return (true);
}
