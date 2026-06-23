/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbonnet <lbonnet@student.42mulhouse.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/03 15:04:07 by lbonnet           #+#    #+#             */
/*   Updated: 2026/06/23 11:29:36 by lbonnet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	destroy_coders(t_sim *sim)
{
	int	i;

	if (!sim->coders)
		return ;
	i = 0;
	while (i < sim->nb_coders)
	{
		pthread_mutex_destroy(&sim->coders[i].coder_mutex);
		i++;
	}
}

void	destroy_dongles(t_sim *sim)
{
	int	i;

	if (!sim->dongles)
		return ;
	i = 0;
	while (i < sim->cleanup.nb_dongles)
	{
		pthread_mutex_destroy(&sim->dongles[i].dongle);
		pthread_mutex_destroy(&sim->dongles[i].heap);
		pthread_mutex_destroy(&sim->dongles[i].state);
		i++;
	}
}

static void	destroy_sync(t_sim *sim)
{
	if (sim->cleanup.print_mutex)
		pthread_mutex_destroy(&sim->print_mutex);
	if (sim->cleanup.sim_mutex)
		pthread_mutex_destroy(&sim->sim_mutex);
	if (sim->cleanup.stop_mutex)
		pthread_mutex_destroy(&sim->stop_mutex);
}

void	destroy_simulation(t_sim *sim)
{
	int	i;

	i = -1;
	if (!sim)
		return ;
	if (sim->monitor)
		pthread_join(sim->monitor, NULL);
	i = -1;
	if (sim->coders)
	{
		while (++i < sim->nb_coders)
			pthread_join(sim->coders[i].coder_thread, NULL);
	}
	destroy_dongles(sim);
	destroy_coders(sim);
	destroy_sync(sim);
	if (sim->coders)
		free(sim->coders);
	if (sim->dongles)
		free(sim->dongles);
}
