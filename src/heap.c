/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heap.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbonnet <lbonnet@student.42mulhouse.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/03 15:04:07 by lbonnet           #+#    #+#             */
/*   Updated: 2026/06/11 11:34:26 by lbonnet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static void	swap(t_request *a, t_request *b)
{
	t_request	temp;

	temp = *a;
	*a = *b;
	*b = temp;
}

void	heap_push(t_heap *heap, t_request request)
{
	int			i;
	int			parent;

	if (heap->size >= heap->capacity)
		return ;
	heap->data[heap->size] = request;
	i = heap->size;
	heap->size++;
	while (i > 0)
	{
		parent = (i - 1) / 2;
		if (heap->data[parent].schedule > heap->data[i].schedule)
		{
			swap(&heap->data[parent], &heap->data[i]);
			i = parent;
		}
		else
			break ;
	}
}

t_request	heap_pop(t_heap *heap)
{
	t_request	node;
	t_request	blank;

	if (heap->size == 0)
	{
		ft_bzero(&blank, sizeof(blank));
		return (blank);
	}
	node = heap->data[0];
	heap->data[0] = heap->data[heap->size - 1];
	heap->size--;
	return (node);
}

t_request	heap_peek(t_heap *heap)
{
	t_request	blank;

	if (heap->size == 0)
	{
		ft_bzero(&blank, sizeof(blank));
		return (blank);
	}
	return (heap->data[0]);
}
