/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbonnet <lbonnet@student.42mulhouse.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/03 15:04:07 by lbonnet           #+#    #+#             */
/*   Updated: 2026/05/29 10:59:41 by lbonnet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	simulation(t_sim *sim)
{
	int	i;

	i = 0;
	while (i < sim->nb_coders)
	{
		pthread_create(sim->list_coders[i]);
	}
}

int	main(int ac, char **av)
{
	t_sim	simulation;

	if (ac != 9)
	{
		print_error(ERROR_MISSING_ARG);
		return (1);
	}
	if (!parser(av, &simulation))
		return (1);
	else
	{
		init_coders(&simulation);
		printf("%s", "Parsing done !\n");
		return (0);
	}
}
