/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dongles.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbonnet <lbonnet@student.42mulhouse.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/03 15:04:07 by lbonnet           #+#    #+#             */
/*   Updated: 2026/06/22 15:25:58 by lbonnet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	lock_dongles(t_coder *coder, t_sim *sim)
{
	while (!scheduler(coder, sim))
		usleep(1);
	if (coder->id % 2 == 1)
	{
		pthread_mutex_lock(&coder->left->dongle);
		pthread_mutex_lock(&coder->right->dongle);
		heap_pop(coder, coder->left);
		heap_pop(coder, coder->right);
	}
	else
	{
		pthread_mutex_lock(&coder->right->dongle);
		pthread_mutex_lock(&coder->left->dongle);
		heap_pop(coder, coder->right);
		heap_pop(coder, coder->left);
	}
	while (get_dongle_state(coder->left, sim)
		|| get_dongle_state(coder->right, sim))
		usleep(1);
	print_status(coder, "has taken a dongle", false);
	print_status(coder, "has taken a dongle", false);
}

void	unlock_dongles(t_coder *coder)
{
	pthread_mutex_lock(&coder->left->state);
	coder->left->last_use = get_time_ms(coder->sim);
	pthread_mutex_unlock(&coder->left->state);
	pthread_mutex_lock(&coder->right->state);
	coder->right->last_use = get_time_ms(coder->sim);
	pthread_mutex_unlock(&coder->right->state);
	pthread_mutex_unlock(&coder->right->dongle);
	pthread_mutex_unlock(&coder->left->dongle);
}

void	check_edf(t_dongle *dongle, t_sim *sim)
{
	pthread_mutex_lock(&dongle->heap);
	if (dongle->waiters[1] && dongle->waiters[0]->compile_start
		+ sim->time_to_burnout > dongle->waiters[1]->compile_start
		+ sim->time_to_burnout)
		swap(dongle);
	pthread_mutex_unlock(&dongle->heap);
}
