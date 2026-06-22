/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   setter.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbonnet <lbonnet@student.42mulhouse.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/03 15:04:07 by lbonnet           #+#    #+#             */
/*   Updated: 2026/06/22 10:28:25 by lbonnet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	set_stop(t_sim *sim, bool value)
{
	pthread_mutex_lock(&sim->sim_mutex);
	sim->stop = value;
	pthread_mutex_unlock(&sim->sim_mutex);
}

void	set_nb_compiles(t_coder *coder, int value)
{
	pthread_mutex_lock(&coder->comp_mutex);
	coder->nb_compiles = value;
	pthread_mutex_unlock(&coder->comp_mutex);
}
