/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbonnet <lbonnet@student.42mulhouse.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/03 15:04:07 by lbonnet           #+#    #+#             */
/*   Updated: 2026/06/10 15:39:06 by lbonnet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	main(int ac, char **av)
{
	t_sim	sim;

	ft_bzero(&sim, sizeof(t_sim));
	if (ac != 9)
	{
		print_error(ERROR_MISSING_ARG);
		return (1);
	}
	if (!parser(av, &sim))
		return (1);
	if (!init_simulation(&sim))
		return (destroy_simulation(&sim), 1);
	simulation(&sim);
	destroy_simulation(&sim);
	return (0);
}

