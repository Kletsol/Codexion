/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbonnet <lbonnet@student.42mulhouse.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/03 15:04:07 by lbonnet           #+#    #+#             */
/*   Updated: 2026/05/20 15:01:32 by lbonnet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

void	*thread_1(void *arg)
{
	int	*i;

	i = (int *) arg;
	(*i) += 5;
	pthread_exit(NULL);
}

int	main(void)
{
	int			i;
	pthread_t	thread1;

	i = 1;
	printf("Avant la création du thread, i = %i.\n", i);

	pthread_create(&thread1, NULL, thread_1, &i);
	pthread_join(thread1, NULL);
	printf("Après la création du thread, i = %i.\n", i);
	return (EXIT_SUCCESS);
}
