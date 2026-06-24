/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   coders.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbonnet <lbonnet@student.42mulhouse.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/03 15:04:07 by lbonnet           #+#    #+#             */
/*   Updated: 2026/06/23 17:33:00 by lbonnet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	*coder_routine(void *arg)
{
	t_coder		*coder;
	uint16_t	compiles;

	coder = (t_coder *)arg;
	compiles = 0;
	if (coder->sim->nb_coders == 1)
		usleep(coder->sim->time_to_burnout * 1000 * 4);
	if (coder->id % 2 == 0)
		usleep((coder->sim->time_to_compile) * 1000);
	while (!get_stop(coder->sim))
	{
		if (!compile(coder))
			return (NULL);
		if (++compiles == coder->sim->nb_compiles)
		{
			pthread_mutex_lock(&coder->sim->stop_mutex);
			coder->sim->finished_coders += 1;
			pthread_mutex_unlock(&coder->sim->stop_mutex);
		}
		if (!debug(coder))
			return (NULL);
		if (!refactor(coder))
			return (NULL);
	}
	return (NULL);
}

bool	compile(t_coder *coder)
{
	if (get_stop(coder->sim))
		return (false);
	lock_dongles(coder, coder->sim);
	print_status(coder, COMPILE, false);
	pthread_mutex_lock(&coder->coder_mutex);
	coder->compile_start = get_time_ms(coder->sim);
	pthread_mutex_unlock(&coder->coder_mutex);
	usleep(coder->sim->time_to_compile * 1000);
	unlock_dongles(coder);
	return (true);
}

bool	debug(t_coder *coder)
{
	if (get_stop(coder->sim))
		return (false);
	print_status(coder, DEBUG, false);
	usleep(coder->sim->time_to_debug * 1000);
	return (true);
}

bool	refactor(t_coder *coder)
{
	if (get_stop(coder->sim))
		return (false);
	print_status(coder, REFACTOR, false);
	usleep(coder->sim->time_to_refactor * 1000);
	return (true);
}
