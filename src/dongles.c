/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dongles.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbonnet <lbonnet@student.42mulhouse.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/03 15:04:07 by lbonnet           #+#    #+#             */
/*   Updated: 2026/06/11 11:33:58 by lbonnet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	wait_dongle(t_coder *coder, t_dongle *dongle)
{
	struct timespec	ts;
	uint64_t		wake_at;

	while (heap_peek(&dongle->waiters).coder_id != coder->id
		|| !dongle->available || get_time_ms() < dongle->end_cooldown)
	{
		if (get_stop(coder->sim))
			break ;
		wake_at = dongle->end_cooldown;
		if (dongle->available && get_time_ms() < wake_at)
		{
			ts.tv_sec = wake_at / 1000;
			ts.tv_nsec = (wake_at % 1000) * 1000000;
			pthread_cond_timedwait(&dongle->cond, &dongle->mutex, &ts);
		}
		else
			pthread_cond_wait(&dongle->cond, &dongle->mutex);
	}
}

bool	take_dongle(t_coder *coder, t_dongle *dongle, uint64_t schedule)
{
	t_request	req;

	req.coder_id = coder->id;
	req.schedule = schedule;
	pthread_mutex_lock(&dongle->mutex);
	heap_push(&dongle->waiters, req);
	wait_dongle(coder, dongle);
	if (get_stop(coder->sim))
		return (pthread_mutex_unlock(&dongle->mutex), false);
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
// void	reserve_dongles(t_coder *coder)
// {
// 	t_dongle	*left;
// 	t_dongle	*right;

// 	left = coder->left_dongle;
// 	right = coder->right_dongle;
// 	lock_dongles(left, right);
// 	left->available = false;
// 	right->available = false;
// 	left->owner = coder;
// 	right->owner = coder;
// 	heap_pop(&left->waiters, coder->sim->policy);
// 	heap_pop(&right->waiters, coder->sim->policy);
// 	unlock_dongles(left, right);
// 	pthread_cond_broadcast(&coder->sim->scheduler_cond);
// }

// bool	request_dongle(t_coder *coder, t_sim *sim)
// {
// 	t_request	req;

// 	req.coder = coder;
// 	req.deadline = coder->last_compile_start + sim->time_to_burnout;
// 	req.seq = get_next_seq(sim);

// 	pthread_mutex_lock(&sim->scheduler_mutex);

// 	heap_push(&coder->left_dongle->waiters, req, sim->policy);
// 	heap_push(&coder->right_dongle->waiters, req, sim->policy);

// 	while (!can_compile(coder) && !simulation_stopped(sim))
// 		pthread_cond_wait(&sim->scheduler_cond, &sim->scheduler_mutex);

// 	if (simulation_stopped(sim))
// 	{
// 		pthread_mutex_unlock(&sim->scheduler_mutex);
// 		return (false);
// 	}

// 	reserve_dongles(coder);

// 	pthread_mutex_unlock(&sim->scheduler_mutex);
// 	return (true);
// }

// bool	request_dongle(t_coder *coder, t_sim *sim)
// {
// 	t_request	req;

// 	req.coder = coder;
// 	req.deadline = coder->last_compile_start + coder->sim->time_to_burnout;
// 	req.seq = get_next_seq(coder->sim);
// 	pthread_mutex_lock(&sim->scheduler_mutex);
// 	heap_push(&coder->left_dongle->waiters, req, sim->policy);
// 	heap_push(&coder->right_dongle->waiters, req, sim->policy);
// 	pthread_cond_broadcast(&sim->scheduler_cond);
// 	while (!can_compile(coder) && !simulation_stopped(sim))
// 		pthread_cond_wait(&sim->scheduler_cond, &sim->scheduler_mutex);

// 	if (simulation_stopped(sim))
// 	{
// 		pthread_mutex_unlock(&sim->scheduler_mutex);
// 		return (false);
// 	}

// 	reserve_dongles(coder);

// 	pthread_mutex_unlock(&sim->scheduler_mutex);
// 	return (true);

// 	while (!simulation_stopped(sim))
// 	{
// 		if (can_compile(coder))
// 		{
// 			lock_dongles(coder->left_dongle, coder->right_dongle);
// 			reserve_dongles(coder);
// 			unlock_dongles(coder->left_dongle, coder->right_dongle);
// 			break ;
// 		}
// 		// printf("Coder %d\n" "  left dongle=%d top=%d\n" "  right dongle=%d top=%d\n", coder->id, coder->left_dongle->id, heap_peek(&coder->left_dongle->waiters)->coder->id, coder->right_dongle->id, heap_peek(&coder->right_dongle->waiters)->coder->id);
// 		pthread_cond_wait(&sim->scheduler_cond, &sim->scheduler_mutex);
// 	}
// 	// printf("coder %d waiting: " "left_top=%d right_top=%d\n", coder->id, heap_peek(&coder->left_dongle->waiters)->coder->id, heap_peek(&coder->right_dongle->waiters)->coder->id);
// 	pthread_mutex_unlock(&sim->scheduler_mutex);
// 	return (false);
// }

// void	lock_dongles(t_dongle *a, t_dongle *b)
// {
// 	if (a->id < b->id)
// 	{
// 		pthread_mutex_lock(&a->mutex);
// 		pthread_mutex_lock(&b->mutex);
// 	}
// 	else
// 	{
// 		pthread_mutex_lock(&b->mutex);
// 		pthread_mutex_lock(&a->mutex);
// 	}
// }

// void	unlock_dongles(t_dongle *a, t_dongle *b)
// {
// 	if (a->id < b->id)
// 	{
// 		pthread_mutex_unlock(&a->mutex);
// 		pthread_mutex_unlock(&b->mutex);
// 	}
// 	else
// 	{
// 		pthread_mutex_unlock(&b->mutex);
// 		pthread_mutex_unlock(&a->mutex);
// 	}
// }

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
		left->end_cooldown = get_time_ms() + coder->sim->cooldown;
		pthread_cond_broadcast(&left->cond);
		pthread_mutex_unlock(&left->mutex);
	}
	if (rel_right)
	{
		pthread_mutex_lock(&right->mutex);
		right->available = true;
		right->end_cooldown = get_time_ms() + coder->sim->cooldown;
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
	}
}
