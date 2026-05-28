/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   initialize.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbonnet <lbonnet@student.42mulhouse.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/03 15:04:07 by lbonnet           #+#    #+#             */
/*   Updated: 2026/05/28 14:57:02 by lbonnet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

bool	init_dongles(t_sim *sim)
{
	int	i;

	sim->dongles = ft_calloc(sim->nb_coders, sizeof(t_dongle));
	if (!sim->dongles)
		return (false);
	i = 0;
	while (i < sim->nb_coders)
	{
		sim->dongles[i].id = i;
		sim->dongles[i].available_at = 0;
		if (pthread_mutex_init(&sim->dongles[i].mutex, NULL) != 0)
		{
			destroy_dongles_mutexes(sim->dongles, i);
			free(sim->dongles);
			return (false);
		}
		i++;
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
		sim->coders[i].last_compile_start = get_time_ms();
		sim->coders[i].left_dongle = &sim->dongles[i];
		sim->coders[i].right_dongle = &sim->dongles[(i + 1) % sim->nb_coders];
		pthread_mutex_init(&sim->coders[i].state_mutex, NULL);
		i++;
	}
	return (true);
}
