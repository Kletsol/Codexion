/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dongles.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbonnet <lbonnet@student.42mulhouse.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/03 15:04:07 by lbonnet           #+#    #+#             */
/*   Updated: 2026/06/15 16:48:59 by lbonnet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

bool	can_take(t_coder *c, t_dongle *d)
{
	return (d->available
		&& get_time_ms() >= d->available_at
		&& heap_peek(&d->waiters).coder_id == c->id);
}

bool	take_dongle(t_coder *coder, t_dongle *dongle, uint64_t schedule)
{
	t_request	req;

	req.coder_id = coder->id;
	req.schedule = schedule;
	pthread_mutex_lock(&dongle->mutex);
	heap_push(&dongle->waiters, req);
	while (!(heap_peek(&dongle->waiters).coder_id == coder->id
			&& dongle->available))
	{
		if (get_stop(coder->sim))
		{
			pthread_mutex_unlock(&dongle->mutex);
			return (false);
		}
		pthread_cond_wait(&dongle->cond, &dongle->mutex);
	}
	while (get_time_ms() < dongle->available_at)
		pthread_cond_wait(&dongle->cond, &dongle->mutex);
	heap_pop(&dongle->waiters);
	dongle->available = false;
	pthread_mutex_unlock(&dongle->mutex);
	return (true);
}

bool	reserve_dongles(t_coder *coder)
{
	uint64_t	scheduler;

	if (coder->sim->nb_coders == 1)
	{
		while (!get_stop(coder->sim))
			usleep(100);
		return (false);
	}
	scheduler = get_policy(coder);
	if (get_stop(coder->sim))
		return (false);
	if (!take_dongle(coder, coder->left_dongle, scheduler))
		return (false);
	print_status(coder, "has taken a dongle");
	if (!take_dongle(coder, coder->right_dongle, scheduler))
		return (release_dongles(coder, true, false), false);
	print_status(coder, "has taken a dongle");
	if (get_stop(coder->sim))
	{
		return (release_dongles(coder, true, true), false);
	}
	return (true);
}

void	release_dongles(t_coder *coder, bool rel_left, bool rel_right)
{
	t_dongle	*left;
	t_dongle	*right;

	left = coder->left_dongle;
	right = coder->right_dongle;
	if (rel_left)
	{
		pthread_mutex_lock(&left->mutex);
		left->available = true;
		left->available_at = get_time_ms() + coder->sim->cooldown;
		pthread_cond_broadcast(&left->cond);
		pthread_mutex_unlock(&left->mutex);
	}
	if (rel_right)
	{
		pthread_mutex_lock(&right->mutex);
		right->available = true;
		right->available_at = get_time_ms() + coder->sim->cooldown;
		pthread_cond_broadcast(&right->cond);
		pthread_mutex_unlock(&right->mutex);
	}
}

void	wake_dongles(t_sim *sim)
{
	int	i;

	i = 0;
	while (i < sim->nb_coders)
	{
		pthread_mutex_lock(&sim->dongles[i].mutex);
		pthread_cond_broadcast(&sim->dongles[i].cond);
		pthread_mutex_unlock(&sim->dongles[i].mutex);
		i++;
	}
}
