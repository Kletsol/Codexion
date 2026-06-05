/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbonnet <lbonnet@student.42mulhouse.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/03 15:04:07 by lbonnet           #+#    #+#             */
/*   Updated: 2026/06/04 15:54:12 by lbonnet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	destroy_dongles(t_sim *sim, int count)
{
	int	i;

	if (!sim->dongles)
		return ;
	i = 0;
	while (i < count)
	{
		free(sim->dongles[i].waiters.data);
		pthread_mutex_destroy(&sim->dongles[i].mutex);
		pthread_cond_destroy(&sim->dongles[i].cond);
		i++;
	}
	free (sim->dongles);
	sim->dongles = NULL;
}

void	destroy_coders(t_sim *sim, int count)
{
	int	i;

	if (!sim->coders)
		return ;
	i = 0;
	while (i < count)
	{
		pthread_mutex_destroy(&sim->coders[i].state_mutex);
		i++;
	}
	free (sim->coders);
	sim->coders = NULL;
}

void	destroy_global_mutexes(t_sim *sim)
{
	pthread_mutex_destroy(&sim->stop_mutex);
	pthread_mutex_destroy(&sim->print_mutex);
}

void	destroy_simulation(t_sim *sim)
{
	if (sim->dongles)
		destroy_dongles(sim, sim->nb_coders);
	if (sim->coders)
		destroy_coders(sim, sim->nb_coders);
	destroy_global_mutexes(sim);
}
