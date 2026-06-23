/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbonnet <lbonnet@student.42mulhouse.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/03 15:04:07 by lbonnet           #+#    #+#             */
/*   Updated: 2026/06/23 10:57:39 by lbonnet          ###   ########.fr       */
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

void	swap(t_dongle *dongle)
{
	t_coder	*temp;

	temp = dongle->waiters[0];
	dongle->waiters[0] = dongle->waiters[1];
	dongle->waiters[1] = temp;
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
