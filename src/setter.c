/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   setter.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbonnet <lbonnet@student.42mulhouse.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/03 15:04:07 by lbonnet           #+#    #+#             */
/*   Updated: 2026/06/11 17:50:44 by lbonnet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	set_stop(t_sim *sim, bool value)
{
	pthread_mutex_lock(&sim->sim_mutex);
	sim->stop = value;
	pthread_mutex_unlock(&sim->sim_mutex);
}

// void	set_state(t_coder *coder, t_state state)
// {
// 	coder->state = state;
// }

void	set_deadline(t_coder *coder, uint64_t deadline)
{
	pthread_mutex_lock(&coder->cod_mutex);
	coder->deadline = deadline;
	pthread_mutex_unlock(&coder->cod_mutex);
}

void	set_nb_compiles(t_coder *coder, int value)
{
	pthread_mutex_lock(&coder->cod_mutex);
	coder->nb_compiles = value;
	pthread_mutex_unlock(&coder->cod_mutex);
}

void	set_finished_coder(t_sim *sim, int value)
{
	pthread_mutex_lock(&sim->sim_mutex);
	sim->finished_coders += value;
	pthread_mutex_unlock(&sim->sim_mutex);
}
