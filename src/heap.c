/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heap.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbonnet <lbonnet@student.42mulhouse.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/03 15:04:07 by lbonnet           #+#    #+#             */
/*   Updated: 2026/06/16 15:57:13 by lbonnet          ###   ########.fr       */
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

static void	sift_down(t_heap *heap, int i)
{
	int	left;
	int	right;
	int	smallest;

	while (1)
	{
		left = i * 2 + 1;
		right = i * 2 + 2;
		smallest = i;
		if (left < heap->size
			&& heap->data[left].priority
			< heap->data[smallest].priority)
			smallest = left;
		if (right < heap->size
			&& heap->data[right].priority
			< heap->data[smallest].priority)
			smallest = right;
		if (smallest == i)
			break ;
		swap(&heap->data[i], &heap->data[smallest]);
		i = smallest;
	}
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
		if (heap->data[parent].priority > heap->data[i].priority)
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
	heap->size--;
	if (heap->size > 0)
	{
		heap->data[0] = heap->data[heap->size];
		sift_down(heap, 0);
	}
	return (node);
}

void	heap_remove(t_heap *heap, int index)
{
	int	parent;

	if (index < 0 || index >= heap->size)
		return ;
	heap->size--;
	if (index == heap->size)
		return ;
	heap->data[index] = heap->data[heap->size];
	parent = (index - 1) / 2;
	if (index > 0
		&& heap->data[index].priority
		< heap->data[parent].priority)
	{
		while (index > 0)
		{
			parent = (index - 1) / 2;
			if (heap->data[parent].priority
				<= heap->data[index].priority)
				break ;
			swap(&heap->data[parent], &heap->data[index]);
			index = parent;
		}
	}
	else
		sift_down(heap, index);
}

// t_request	heap_pop_valid(t_heap *h)
// {
// 	t_request	r;

// 	while (h->size > 0)
// 	{
// 		r = heap_pop(h);
// 		if (r.valid)
// 			return (r);
// 	}

// 	ft_bzero(&r, sizeof(r));
// 	return (r);
// }
