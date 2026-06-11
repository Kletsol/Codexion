/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   initialize.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbonnet <lbonnet@student.42mulhouse.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/03 15:04:07 by lbonnet           #+#    #+#             */
/*   Updated: 2026/06/11 17:40:19 by lbonnet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

bool	init_dongles(t_sim *sim)
{
	int	i;

	i = 0;
	while (i < sim->nb_coders)
	{
		sim->dongles[i].available = true;
		sim->dongles[i].waiters.capacity = 2;
		if (pthread_mutex_init(&sim->dongles[i].mutex, NULL) != 0)
			return (print_error("Failed to initialize mutex"));
		sim->cleanup.dongles_mutex += 1;
		if (pthread_cond_init(&sim->dongles[i].cond, NULL) != 0)
			return (print_error("Failed to initialize condition"));
		sim->cleanup.dongles_cond += 1;
		i++;
	}
	return (true);
}

void	swap_hands(t_coder *coder)
{
	t_dongle	*temp;

	if (coder->id % 2 == 1)
	{
		temp = coder->right_dongle;
		coder->right_dongle = coder->left_dongle;
		coder->left_dongle = temp;
	}
}

bool	init_coders(t_sim *sim)
{
	int	i;

	i = 0;
	while (i < sim->nb_coders)
	{
		sim->coders[i].id = i + 1;
		sim->coders[i].deadline = UINT64_MAX;
		sim->coders[i].left_dongle = &sim->dongles[i];
		if (i == sim->nb_coders - 1)
			sim->coders[i].right_dongle = &sim->dongles[0];
		else
			(sim->coders[i].right_dongle = &sim->dongles[i + 1]);
		sim->coders[i].sim = sim;
		if (pthread_mutex_init(&sim->coders[i].cod_mutex, NULL) != 0)
			return (print_error("Failed to initialize mutex"));
		swap_hands(&sim-> coders[i]);
		i++;
	}
	return (true);
}

bool	init_simulation(t_sim *sim)
{
	sim->coders = ft_calloc(sim->nb_coders, sizeof(t_coder));
	if (!sim->coders)
		return (print_error("Allocation failed for coders tab"));
	sim->dongles = ft_calloc(sim->nb_coders, sizeof(t_dongle));
	if (!sim->dongles)
		return (print_error("Allocation failed for dongles tab"));
	if (pthread_mutex_init(&sim->stop_mutex, NULL) != 0)
		return (print_error("Failed to initialize mutex"));
	sim->cleanup.stop_mutex = true;
	if (pthread_mutex_init(&sim->print_mutex, NULL) != 0)
		return (print_error("Failed to initialize mutex"));
	sim->cleanup.print_mutex = true;
	if (pthread_mutex_init(&sim->sim_mutex, NULL) != 0)
		return (print_error("Failed to initialize mutex"));
	sim->cleanup.sim_mutex = true;
	if (!init_dongles(sim))
		return (false);
	if (!init_coders(sim))
		return (false);
	return (true);
}
