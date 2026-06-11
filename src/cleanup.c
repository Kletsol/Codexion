/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbonnet <lbonnet@student.42mulhouse.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/03 15:04:07 by lbonnet           #+#    #+#             */
/*   Updated: 2026/06/09 16:56:55 by lbonnet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	join_coders(t_sim *sim)
{
	int	i;

	i = 0;
	while (i < sim->nb_coders)
	{
		pthread_join(sim->coders[i].cod_thread, NULL);
		pthread_mutex_destroy(&sim->coders[i].cod_mutex);
		i++;
	}
}


void	destroy_simulation(t_sim *sim)
{
	int	i;

	pthread_join(sim->monitor, NULL);
	join_coders(sim);
	if (sim->coders)
		free(sim->coders);
	i = 0;
	while (i < sim->cleanup.dongles_cond)
		pthread_cond_destroy(&sim->dongles[i++].cond);
	i = 0;
	while (i < sim->cleanup.dongles_mutex)
		pthread_mutex_destroy(&sim->dongles[i++].mutex);
	if (sim->dongles)
		free(sim->dongles);
	if (sim->cleanup.print_mutex)
		pthread_mutex_destroy(&sim->print_mutex);
	if (sim->cleanup.sim_mutex)
		pthread_mutex_destroy(&sim->sim_mutex);
	if (sim->cleanup.stop_mutex)
		pthread_mutex_destroy(&sim->stop_mutex);
}
