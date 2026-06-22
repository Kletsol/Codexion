/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   coders.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbonnet <lbonnet@student.42mulhouse.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/03 15:04:07 by lbonnet           #+#    #+#             */
/*   Updated: 2026/06/22 16:40:31 by lbonnet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	*coder_routine(void *arg)
{
	t_coder	*coder;
	int		compiles;

	coder = (t_coder *)arg;
	compiles = 1;
	if (coder->sim->nb_coders == 1)
		usleep(coder->sim->time_to_burnout * 1000 * 4);
	if (coder->id % 2 == 0)
		usleep((coder->sim->time_to_compile) * 1000);
	while (!get_stop(coder->sim))
	{
		compile(coder);
		debug(coder);
		refactor(coder);
		compiles++;
		if (compiles == coder->sim->nb_compiles)
		{
			pthread_mutex_lock(&coder->sim->stop_mutex);
			coder->sim->finished_coders += 1;
			pthread_mutex_unlock(&coder->sim->stop_mutex);
		}
	}
	return (NULL);
}

bool	compile(t_coder *coder)
{
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
