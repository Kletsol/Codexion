/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbonnet <lbonnet@student.42mulhouse.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/03 15:04:07 by lbonnet           #+#    #+#             */
/*   Updated: 2026/06/03 14:37:40 by lbonnet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	check_burnout(t_sim *sim)
{
	int			i;
	uint64_t	elapsed;
	uint64_t	time;

	i = 0;
	while (i < sim->nb_coders)
	{
		pthread_mutex_lock(&sim->coders[i].state_mutex);
		elapsed = get_time_ms() - sim->coders[i].last_compile_start;
		if (elapsed >= sim->time_to_burnout)
		{
			time = elapsed_time(sim->coders[i].sim);
			sim->coders[i].state = BURNED_OUT;
			set_stop(sim, true);
			pthread_mutex_unlock(&sim->coders[i].state_mutex);
			pthread_mutex_lock(&sim->print_mutex);
			printf("%lu %d burned out\n", time, sim->coders[i].id);
			pthread_mutex_unlock(&sim->print_mutex);
			return ;
		}
		pthread_mutex_unlock(&sim->coders[i].state_mutex);
		i++;
	}
}

bool	check_completion(t_sim *sim)
{
	int		i;

	i = 0;
	while (i < sim->nb_coders)
	{
		pthread_mutex_lock(&sim->coders[i].state_mutex);
		if (sim->coders[i].nb_compiles < sim->nb_compiles)
		{
			pthread_mutex_unlock(&sim->coders[i].state_mutex);
			return (false);
		}
		pthread_mutex_unlock(&sim->coders[i].state_mutex);
		i++;
	}
	return (true);
}

void	*monitor_routine(void *arg)
{
	t_sim	*sim;

	sim = (t_sim *)arg;
	while (!simulation_stopped(sim))
	{
		check_burnout(sim);
		if (simulation_stopped(sim))
			break ;
		if (check_completion(sim))
		{
			set_stop(sim, true);
			break ;
		}
		usleep(100);
	}
	return (NULL);
}
