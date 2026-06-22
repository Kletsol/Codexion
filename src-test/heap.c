/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heap.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbonnet <lbonnet@student.42mulhouse.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/03 15:04:07 by lbonnet           #+#    #+#             */
/*   Updated: 2026/06/22 14:23:59 by lbonnet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	swap(t_dongle *dongle)
{
	t_coder	*temp;

	temp = dongle->waiters[0];
	dongle->waiters[0] = dongle->waiters[1];
	dongle->waiters[1] = temp;
}

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
