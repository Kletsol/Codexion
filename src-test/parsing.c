/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbonnet <lbonnet@student.42mulhouse.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/03 15:04:07 by lbonnet           #+#    #+#             */
/*   Updated: 2026/06/18 14:14:52 by lbonnet          ###   ########.fr       */
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

bool	valid_policy(char *str, t_sim *simulation)
{
	if (!strcmp(str, "edf"))
		simulation->policy = EDF;
	else if (!strcmp(str, "fifo"))
		simulation->policy = FIFO;
	else
		return (print_error(ERROR_POLICY));
	return (true);
}

bool	parser(char **av, t_sim *simulation)
{
	if (!valid_number(&simulation->nb_coders, av[1], sizeof(int))
		||!valid_number(&simulation->time_to_burnout, av[2], sizeof(uint64_t))
		||!valid_number(&simulation->time_to_compile, av[3], sizeof(uint64_t))
		||!valid_number(&simulation->time_to_debug, av[4], sizeof(uint64_t))
		||!valid_number(&simulation->time_to_refactor, av[5], sizeof(uint64_t))
		||!valid_number(&simulation->nb_compiles, av[6], sizeof(int))
		||!valid_number(&simulation->cooldown, av[7], sizeof(uint64_t))
		||!valid_policy(av[8], simulation))
		return (false);
	if (simulation->nb_coders <= 0)
		return (print_error(ERROR_NB_CODERS));
	if (simulation->nb_compiles <= 0)
		return (print_error(ERROR_NB_COMPILES));
	return (true);
}
