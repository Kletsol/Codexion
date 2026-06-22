/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   logs.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbonnet <lbonnet@student.42mulhouse.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/03 15:04:07 by lbonnet           #+#    #+#             */
/*   Updated: 2026/06/22 15:28:04 by lbonnet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

// uint64_t	elapsed_time(t_sim *sim)
// {
// 	return (get_time_ms(sim) - sim->start_time);
// }

void	print_status(t_coder *coder, char *str, bool burnout)
{
	pthread_mutex_lock(&coder->sim->print_mutex);
	if (!coder->sim->silence || burnout == true)
		printf("%lu %d %s\n", get_time_ms(coder->sim), coder->id, str);
	pthread_mutex_unlock(&coder->sim->print_mutex);
}
