/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simulation.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbonnet <lbonnet@student.42mulhouse.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/03 15:04:07 by lbonnet           #+#    #+#             */
/*   Updated: 2026/06/22 16:43:06 by lbonnet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	simulation(t_sim *sim)
{
	int	i;

	i = 0;
	if (pthread_create(&sim->monitor, NULL, monitor_routine, sim) != 0)
		return ;
	while (i < sim->nb_coders)
	{
		if (pthread_create(&sim->coders[i].coder_thread, NULL,
				coder_routine, &sim->coders[i]) != 0)
			return ;
		i++;
	}
}
