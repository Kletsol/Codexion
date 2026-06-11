/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simulation.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbonnet <lbonnet@student.42mulhouse.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/03 15:04:07 by lbonnet           #+#    #+#             */
/*   Updated: 2026/06/10 15:03:47 by lbonnet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	simulation(t_sim *sim)
{
	int	i;

	i = 0;
	sim->start_time = get_time_ms();
	pthread_create(&sim->monitor, NULL, monitor_routine, sim);
	while (i < sim->nb_coders)
	{
		pthread_create(&sim->coders[i].cod_thread, NULL,
			coder_routine, &sim->coders[i]);
		i++;
	}
}
