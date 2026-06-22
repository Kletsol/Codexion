/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   coders.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbonnet <lbonnet@student.42mulhouse.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/03 15:04:07 by lbonnet           #+#    #+#             */
/*   Updated: 2026/06/20 12:13:54 by lbonnet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	*coder_routine(void *arg)
{
	t_coder	*coder;
	int		compiles;

	coder = (t_coder *)arg;
	set_deadline(coder, get_time_ms() + coder->sim->time_to_burnout);
	compiles = get_nb_compiles(coder);
	while (compiles < coder->sim->nb_compiles)
	{
		if (!can_compile(coder))
			return (NULL);
		set_nb_compiles(coder, ++compiles);
		if (!debug(coder))
			return (NULL);
		if (!refactor(coder))
			return (NULL);
	}
	set_finished_coder(coder->sim, 1);
	return (NULL);
}

bool	debug(t_coder *coder)
{
	if (get_stop(coder->sim))
		return (false);
	print_status(coder, "is debugging");
	smart_sleep(coder->sim->time_to_debug, coder->sim);
	return (true);
}

bool	refactor(t_coder *coder)
{
	if (get_stop(coder->sim))
		return (false);
	print_status(coder, "is refactoring");
	smart_sleep(coder->sim->time_to_refactor, coder->sim);
	return (true);
}

bool	can_compile(t_coder *coder)
{
	if (get_stop(coder->sim))
		return (false);
	if (!reserve_dongles(coder))
		return (false);
	pthread_mutex_lock(&coder->death_mutex);
	coder->compile_start = get_time_ms();
	pthread_mutex_unlock(&coder->death_mutex);
	set_deadline(coder, coder->compile_start + coder->sim->time_to_burnout);
	print_status(coder, "is compiling");
	smart_sleep(coder->sim->time_to_compile, coder->sim);
	release_dongles(coder, true, true);
	return (true);
}
