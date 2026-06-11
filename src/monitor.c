/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbonnet <lbonnet@student.42mulhouse.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/03 15:04:07 by lbonnet           #+#    #+#             */
/*   Updated: 2026/06/11 17:56:40 by lbonnet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

bool	check_burnout(t_sim *sim, uint64_t time)
{
	int			i;
	int			nb_compiles;
	int			finished;
	uint64_t	deadline;

	i = 0;
	while (i < sim->nb_coders)
	{
		nb_compiles = get_nb_compiles(&sim->coders[i]);
		deadline = get_deadline(&sim->coders[i]);
		if (time > deadline && nb_compiles != sim->nb_compiles)
		{
			print_status(&sim->coders[i], "burned out");
			return (true);
		}
		i++;
	}
	finished = get_finished_coders(sim);
	if (sim->nb_coders == finished)
		return (true);
	return (false);
}

// bool	check_completion(t_sim *sim)
// {
// 	int		i;

// 	i = 0;
// 	while (i < sim->nb_coders)
// 	{
// 		pthread_mutex_lock(&sim->coders[i].state_mutex);
// 		if (sim->coders[i].nb_compiles < sim->nb_compiles)
// 		{
// 			pthread_mutex_unlock(&sim->coders[i].state_mutex);
// 			return (false);
// 		}
// 		pthread_mutex_unlock(&sim->coders[i].state_mutex);
// 		i++;
// 	}
// 	return (true);
// }

void	*monitor_routine(void *arg)
{
	t_sim		*sim;
	uint64_t	time;

	sim = (t_sim *)arg;
	while (1)
	{
		usleep(100);
		if (get_stop(sim))
			return (NULL);
		time = get_time_ms();
		if (check_burnout(sim, time))
		{
			set_stop(sim, true);
			wake_dongles(sim);
			return (NULL);
		}
	}
	return (NULL);
}
