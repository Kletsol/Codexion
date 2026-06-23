/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbonnet <lbonnet@student.42mulhouse.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/03 15:04:07 by lbonnet           #+#    #+#             */
/*   Updated: 2026/06/23 16:53:13 by lbonnet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

bool	check_burnout(t_sim *sim)
{
	int	i;

	i = 0;
	while (i < sim->nb_coders)
	{
		pthread_mutex_lock(&sim->coders[i].coder_mutex);
		if (get_time_ms(sim) - sim->coders[i].compile_start
			> sim->time_to_burnout)
		{
			pthread_mutex_lock(&sim->print_mutex);
			sim->silence = true;
			pthread_mutex_unlock(&sim->print_mutex);
			usleep(1000);
			set_stop(sim);
			print_status(&sim->coders[i], BURNOUT, true);
			return (pthread_mutex_unlock(&sim->coders[i].coder_mutex), true);
		}
		pthread_mutex_unlock(&sim->coders[i].coder_mutex);
		i++;
	}
	return (false);
}

void	*monitor_routine(void *arg)
	{
	t_sim	*sim;

	sim = (t_sim *)arg;
	while (!get_stop(sim))
	{
		pthread_mutex_lock(&sim->stop_mutex);
		if (sim->nb_coders != 1 && sim->finished_coders >= sim->nb_coders)
			set_stop(sim);
		pthread_mutex_unlock(&sim->stop_mutex);
		if (check_burnout(sim))
			break ;
		usleep(1);
	}
	return (NULL);
}
