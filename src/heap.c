/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heap.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbonnet <lbonnet@student.42mulhouse.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/03 15:04:07 by lbonnet           #+#    #+#             */
/*   Updated: 2026/06/23 10:58:34 by lbonnet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	heap_push(t_coder *coder, t_dongle *dongle)
{
	pthread_mutex_lock(&dongle->heap);
	if (!dongle->waiters[0] || dongle->waiters[0]->id == coder->id)
		dongle->waiters[0] = coder;
	else if (!dongle->waiters[1])
		dongle->waiters[1] = coder;
	pthread_mutex_unlock(&dongle->heap);
}

void	heap_pop(t_coder *coder, t_dongle *dongle)
{
	pthread_mutex_lock(&dongle->heap);
	if (dongle->waiters[0]->id == coder->id)
	{
		swap(dongle);
		dongle->waiters[1] = NULL;
	}
	else if (dongle->waiters[1]->id == coder->id)
		dongle->waiters[1] = NULL;
	pthread_mutex_unlock(&dongle->heap);
	return ;
}

int	heap_peek(t_dongle *dongle)
{
	int	id;

	id = -1;
	pthread_mutex_lock(&dongle->heap);
	if (dongle->waiters[0])
		id = dongle->waiters[0]->id;
	pthread_mutex_unlock(&dongle->heap);
	return (id);
}

bool	scheduler(t_coder *coder, t_sim *sim)
{
	heap_push(coder, coder->left);
	heap_push(coder, coder->right);
	if (sim->policy)
	{
		check_edf(coder->left, sim);
		check_edf(coder->right, sim);
	}
	if (heap_peek(coder->right) == coder->id
		&& heap_peek(coder->left) == coder->id)
		return (true);
	return (false);
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
