/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbonnet <lbonnet@student.42mulhouse.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/03 15:04:07 by lbonnet           #+#    #+#             */
/*   Updated: 2026/06/10 15:39:53 by lbonnet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

uint64_t	ft_atou(const char *nptr, bool *error)
{
	int			i;
	uint64_t	res;

	res = 0;
	i = 0;
	while ((nptr[i] >= 9 && nptr[i] <= 13) || nptr[i] == 32)
		i++;
	if (nptr[i] == '+')
		i++;
	else if (nptr[i] == '-')
		return (*error = true);
	while (nptr[i] >= '0' && nptr[i] <= '9')
	{
		if (res > (UINT64_MAX - (nptr[i] - 48)) / 10)
			return (*error = true);
		res = res * 10 + (nptr[i] - 48);
		i++;
	}
	return (res);
}

uint64_t	get_time_ms(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return ((uint64_t)tv.tv_sec * 1000 + (uint64_t)tv.tv_usec / 1000);
}

void	smart_sleep(uint64_t duration)
{
	uint64_t	start;
	uint64_t	time;

	start = get_time_ms();
	time = start;
	while (time - start < duration)
	{
		usleep(100);
		time = get_time_ms();
	}
}

// uint64_t	get_next_seq(t_sim *sim)
// {
// 	uint64_t	seq;

// 	seq = 0;
// 	pthread_mutex_lock(&sim->seq_mutex);
// 	seq = sim->next_seq++;
// 	pthread_mutex_unlock(&sim->seq_mutex);
// 	return (seq);
// }
