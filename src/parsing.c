/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbonnet <lbonnet@student.42mulhouse.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/03 15:04:07 by lbonnet           #+#    #+#             */
/*   Updated: 2026/06/23 16:43:13 by lbonnet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	ft_isdigit(int n)
{
	if (n < '0' || n > '9')
		return (0);
	return (1);
}

bool	ft_striter(char *str, int (*f)(int))
{
	size_t	i;

	i = 0;
	if (!str || !f)
		return (false);
	while (str[i] != '\0')
	{
		if (!f(str[i]))
			return (false);
		i++;
	}
	return (true);
}

bool	valid_number(void *dest, char *str, size_t size)
{
	bool		error;
	int			*int_dest;
	uint64_t	*uint64_dest;

	error = false;
	if (!ft_striter(str, ft_isdigit))
		return (print_error(ERROR_NOT_POS_INT));
	if (size == sizeof(int))
	{
		int_dest = (int *)dest;
		*int_dest = ft_atou(str, &error);
	}
	else if (size == sizeof(uint64_t))
	{
		uint64_dest = (uint64_t *)dest;
		*uint64_dest = ft_atou(str, &error);
	}
	if (error == true)
		return (print_error(ERROR_OVERFLOW));
	return (true);
}

bool	valid_policy(char *str, t_sim *sim)
{
	if (!strcmp(str, "edf"))
		sim->policy = EDF;
	else if (!strcmp(str, "fifo"))
		sim->policy = FIFO;
	else
		return (print_error(ERROR_POLICY));
	return (true);
}

bool	parser(char **av, t_sim *sim)
{
	if (!valid_number(&sim->nb_coders, av[1], sizeof(int))
		||!valid_number(&sim->time_to_burnout, av[2], sizeof(uint64_t))
		||!valid_number(&sim->time_to_compile, av[3], sizeof(uint64_t))
		||!valid_number(&sim->time_to_debug, av[4], sizeof(uint64_t))
		||!valid_number(&sim->time_to_refactor, av[5], sizeof(uint64_t))
		||!valid_number(&sim->nb_compiles, av[6], sizeof(int))
		||!valid_number(&sim->cooldown, av[7], sizeof(uint64_t))
		||!valid_policy(av[8], sim))
		return (false);
	if (sim->nb_coders <= 0)
		return (print_error(ERROR_NB_CODERS));
	if (sim->nb_compiles <= 0)
		return (print_error(ERROR_NB_COMPILES));
	if (sim->time_to_compile < 1 || sim->time_to_debug < 1
		|| sim->time_to_refactor < 1 || sim->cooldown < 1)
		return (print_error(ERROR_INT_TOO_LOW));
	return (true);
}
