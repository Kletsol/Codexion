/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   getter.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbonnet <lbonnet@student.42mulhouse.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/03 15:04:07 by lbonnet           #+#    #+#             */
/*   Updated: 2026/06/10 16:46:53 by lbonnet          ###   ########.fr       */
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

int	get_nb_compiles(t_coder *coder)
{
	int	result;

	pthread_mutex_lock(&coder->cod_mutex);
	result = coder->nb_compiles;
	pthread_mutex_unlock(&coder->cod_mutex);
	return (result);
}

uint64_t	get_deadline(t_coder *coder)
{
	uint64_t	result;

	pthread_mutex_lock(&coder->cod_mutex);
	result = coder->deadline;
	pthread_mutex_unlock(&coder->cod_mutex);
	return (result);
}

int	get_finished_coders(t_sim *sim)
{
	int	result;

	pthread_mutex_lock(&sim->stop_mutex);
	result = sim->finished_coders;
	pthread_mutex_unlock(&sim->stop_mutex);
	return (result);
}

uint64_t	get_policy(t_coder *coder)
{
	if (coder->sim->policy)
		return (coder->compile_start + coder->sim->time_to_burnout);
	return (get_time_ms());
}
