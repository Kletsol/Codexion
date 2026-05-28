/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   memory.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbonnet <lbonnet@student.42mulhouse.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/03 15:04:07 by lbonnet           #+#    #+#             */
/*   Updated: 2026/05/28 15:57:27 by lbonnet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	ft_bzero(void *s, size_t n)
{
	memset(s, 0, n);
}

void	*ft_calloc(size_t nmemb, size_t size)
{
	size_t	total_size;
	void	*new;

	total_size = nmemb * size;
	if (size != 0 && total_size / size != nmemb)
		return (NULL);
	new = malloc(total_size);
	if (!new)
		return (NULL);
	ft_bzero(new, total_size);
	return (new);
}
