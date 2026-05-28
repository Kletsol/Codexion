/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test3.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbonnet <lbonnet@student.42mulhouse.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/03 15:04:07 by lbonnet           #+#    #+#             */
/*   Updated: 2026/05/22 11:27:43 by lbonnet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <pthread.h>
#include <stdio.h>
#include <string.h>

void	*worker(void *arg)
{
	int	n;

	n = *(int *)arg;
	printf("Thread reçoit : %d\n", n);
	return (NULL);
}

int	main(void)
{
	pthread_t	tid;
	int			val;
	int			err;

	val = 42;
	err = pthread_create(&tid, NULL, worker, &val);
	if (err != 0)
	{
		fprintf(stderr, "pthread_create: %s\n", strerror(err));
		return (1);
	}
	pthread_join(tid, NULL);
	return (0);
}
