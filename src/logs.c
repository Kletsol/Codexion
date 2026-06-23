/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   logs.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbonnet <lbonnet@student.42mulhouse.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/03 15:04:07 by lbonnet           #+#    #+#             */
/*   Updated: 2026/06/23 10:22:29 by lbonnet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	print_status(t_coder *coder, char *str, bool burnout)
{
	pthread_mutex_lock(&coder->sim->print_mutex);
	if (!coder->sim->silence || burnout == true)
		printf("%lu %d %s\n", get_time_ms(coder->sim), coder->id, str);
	pthread_mutex_unlock(&coder->sim->print_mutex);
}
