/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbonnet <lbonnet@student.42mulhouse.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/03 15:04:07 by lbonnet           #+#    #+#             */
/*   Updated: 2026/05/25 15:01:48 by lbonnet          ###   ########.fr       */
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
