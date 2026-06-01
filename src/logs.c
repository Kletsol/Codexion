/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   logs.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbonnet <lbonnet@student.42mulhouse.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/03 15:04:07 by lbonnet           #+#    #+#             */
/*   Updated: 2026/06/01 14:11:03 by lbonnet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

uint64_t	elapsed_time(t_sim *sim)
{
	return (get_time_ms() - sim->start_time);
}

void	print_status(t_coder *coder, char *str)
{
	pthread_mutex_lock(&coder->sim->print_mutex);
	if (!simulation_stopped(coder->sim))
	{
		printf("%lu %d %s\n", elapsed_time(coder->sim), coder->id, str);
	}
	pthread_mutex_unlock(&coder->sim->print_mutex);
}
