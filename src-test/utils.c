/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbonnet <lbonnet@student.42mulhouse.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/03 15:04:07 by lbonnet           #+#    #+#             */
/*   Updated: 2026/06/22 10:23:03 by lbonnet          ###   ########.fr       */
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
		return (*error = true, 0);
	while (nptr[i] >= '0' && nptr[i] <= '9')
	{
		if (res > (UINT64_MAX - (nptr[i] - 48)) / 10)
			return (*error = true, 0);
		res = res * 10 + (nptr[i] - 48);
		i++;
	}
	return (res);
}

uint64_t	get_time_ms(t_sim *sim)
{
	struct timeval	tv;
	uint64_t		p_sec;
	uint64_t		p_usec;
	uint64_t		s_sec;
	uint64_t		s_usec;

	gettimeofday(&tv, NULL);
	p_sec = tv.tv_sec * 1000;
	p_usec = tv.tv_usec / 1000;
	s_sec = sim->start_time.tv_sec * 1000;
	s_usec = sim->start_time.tv_usec / 1000;
	return ((p_sec + p_usec) - (s_sec + s_usec));
}

void	smart_sleep(uint64_t duration, t_sim *sim)
{
	uint64_t	start;
	uint64_t	time;

	start = get_time_ms(sim);
	time = start;
	while (time - start < duration && !get_stop(sim))
	{
		usleep(100);
		time = get_time_ms(sim);
	}
}

