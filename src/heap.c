/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heap.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbonnet <lbonnet@student.42mulhouse.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/03 15:04:07 by lbonnet           #+#    #+#             */
/*   Updated: 2026/06/05 10:51:21 by lbonnet          ###   ########.fr       */
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

bool	request_priority(t_request *a, t_request *b, t_enum_sched policy)
{
	if (policy == FIFO)
	{
		if (a->request_time != b->request_time)
			return (a->request_time < b->request_time);
	}
	else
	{
		if (a->deadline != b->deadline)
			return (a->deadline < b->deadline);
	}
	return (a->seq < b->seq);
}


bool	heap_push(t_heap *heap, t_request request, t_enum_sched policy)
{
	int			i;
	int			parent;

	if (heap->size >= heap->capacity)
		return (false);
	heap->data[heap->size] = request;
	i = heap->size;
	heap->size++;
	while (i > 0)
	{
		parent = (i - 1) / 2;
		if (request_priority(&heap->data[i], &heap->data[parent], policy))
		{
			swap(&heap->data[parent], &heap->data[i]);
			i = parent;
		}
		else
			break ;
	}
	return (true);
}

t_request	heap_pop(t_heap *heap, t_enum_sched policy)
{
	t_request	node;
	int			i;
	int			smallest;

	i = 0;
	if (heap->size == 0)
		return ((t_request){0});
	node = heap->data[0];
	heap->data[0] = heap->data[heap->size - 1];
	heap->size--;
	while (true)
	{
		smallest = i;
		if (2 * i + 1 < heap->size && request_priority(
				&heap->data[2 * i + 1], &heap->data[smallest], policy))
			smallest = 2 * i + 1;
		if (2 * i + 2 < heap->size && request_priority(
				&heap->data[2 * i + 2], &heap->data[smallest], policy))
			smallest = 2 * i + 2;
		if (smallest == i)
			break ;
		swap(&heap->data[i], &heap->data[smallest]);
		i = smallest;
	}
	return (node);
}

t_request	*heap_peek(t_heap *heap)
{
	if (heap->size == 0)
		return (NULL);
	return (&heap->data[0]);
}
