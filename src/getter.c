/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   getter.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbonnet <lbonnet@student.42mulhouse.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/03 15:04:07 by lbonnet           #+#    #+#             */
/*   Updated: 2026/06/23 16:16:48 by lbonnet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

bool	get_stop(t_sim *sim)
{
	bool	result;

	pthread_mutex_lock(&sim->sim_mutex);
	result = sim->stop;
	pthread_mutex_unlock(&sim->sim_mutex);
	if (result)
		return (true);
	return (false);
}

bool	get_dongle_state(t_dongle *dongle, t_sim *sim)
{
	bool	available;

	pthread_mutex_lock(&dongle->state);
	if (dongle->last_use == 0
		|| get_time_ms(sim) - dongle->last_use >= sim->cooldown)
		available = false;
	else
		available = true;
	pthread_mutex_unlock(&dongle->state);
	return (available);
}
