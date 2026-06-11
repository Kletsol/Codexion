/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   coders.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbonnet <lbonnet@student.42mulhouse.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/03 15:04:07 by lbonnet           #+#    #+#             */
/*   Updated: 2026/06/11 17:52:04 by lbonnet          ###   ########.fr       */
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
		if (!debug(coder))
			return (NULL);
		if (!refactor(coder))
			return (NULL);
		set_nb_compiles(coder, ++compiles);
	}
	set_finished_coder(coder->sim, 1);
	return (NULL);
}

bool	debug(t_coder *coder)
{
	if (get_stop(coder->sim))
		return (false);
	print_status(coder, "is debugging");
	smart_sleep(coder->sim->time_to_debug);
	return (true);
}

bool	refactor(t_coder *coder)
{
	if (get_stop(coder->sim))
		return (false);
	print_status(coder, "is refactoring");
	smart_sleep(coder->sim->time_to_refactor);
	return (true);
}

// bool	start_coders(t_sim *sim)
// {
// 	int	i;

// 	i = 0;
// 	while (i < sim->nb_coders)
// 	{
// 		if (pthread_create(&sim->coders[i].thread, NULL,
// 				coder_routine, &sim->coders[i]) != 0)
// 			return (false);
// 		i++;
// 	}
// 	return (true);
// }

// void	wait_threads(t_sim *sim)
// {
// 	int	i;

// 	i = 0;
// 	while (i < sim->nb_coders)
// 	{
// 		pthread_join(sim->coders[i].thread, NULL);
// 		i++;
// 	}
// 	pthread_join(sim->monitor, NULL);
// }

bool	can_compile(t_coder *coder)
{
	if (get_stop(coder->sim))
		return (false);
	if (!reserve_dongles(coder))
		return (false);
	if (get_stop(coder->sim))
		return (false);
	coder->compile_start = get_time_ms();
	set_deadline(coder, get_time_ms() + coder->sim->time_to_burnout);
	print_status(coder, "is compiling");
	smart_sleep(coder->sim->time_to_compile);
	release_dongles(coder, true, true);
	return (true);
}
