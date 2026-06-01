/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbonnet <lbonnet@student.42mulhouse.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/03 15:04:07 by lbonnet           #+#    #+#             */
/*   Updated: 2026/06/01 17:23:48 by lbonnet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

bool	check_burnout(t_sim *sim)
{
	int	i;

	i = 0;
	while (i < sim->nb_coders)
	{
		pthread_mutex_lock(&coders[i]->state_mutex)
		elapsed = get_time_ms() - coders[i]->last_compile_start;
		if (elapsed >= sim->time_to_burnout)
		{
			coders[i]->state = BURNED_OUT;
			pthread_mutex_unlock(&coders[i]->state_mutex);
			print_status();
			return (true);
		}
		pthread_mutex_unlock(&coders[i]->state_mutex);
		i++;
	}
	return (false);
}

bool	check_completion(t_sim *sim)
{
	int	i;

	i = 0;
	all_done = true;
	while (i < sim->nb_coders)
	{
		pthread_mutex_lock(&sim->coders[i].state_mutex);
		if (sim->coders[i].nb_compiles < sim->nb_compiles)
			all_done = false;
		pthread_mutex_unlock(&sim->coders[i].state_mutex);
		if (!all_done)
			break ;
		i++;
	}
	return (all_done);
}

void	*monitor_routine(void *arg)
{
	t_sim	*sim;

	sim = (t_sim *)arg;
	while (!simulation_stopped(sim))
	{
		if (check_burnout(sim))
		{
			set_stop(sim, true);
			break ;
		}
		if (check_completion(sim))
			set_stop(sim, true);
		usleep(100);
	}
	return (NULL);
}
