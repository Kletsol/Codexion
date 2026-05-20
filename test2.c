/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test2.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbonnet <lbonnet@student.42mulhouse.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/03 15:04:07 by lbonnet           #+#    #+#             */
/*   Updated: 2026/05/20 16:17:29 by lbonnet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

// Nombre total de thread
#define NB_THREAD 2
// Limite de l'incrément
#define INCREMENT_LIMIT 10

// Tableau contenant les threads
pthread_t	threads[NB_THREAD];

// Structure de données contenant le mutex
typedef struct mutex_data {
	int data;
	pthread_mutex_t mutex;
} mutex_data;

// Fonction exécutée dans le thread
void	*job(void *arg)
{
	mutex_data	*md;
	pthread_t	tid;

	md = (mutex_data *) arg;
	tid = pthread_self();
	while ((*md).data < INCREMENT_LIMIT)
	{
		pthread_mutex_lock(&(*md).mutex);
		(*md).data++;
		pthread_mutex_unlock(&(*md).mutex);
		printf("thread [ %ld ] data [ %i ]\n", tid, (*md).data);
		sleep(1);
	}
	printf("Fin du thread %ld\n", tid);
	pthread_exit(NULL);
}

// Fonction principale
int	main(void)
{
	mutex_data	md;
	int			err;

	md.data = 0;
	if (pthread_mutex_init(&md.mutex, NULL) != 0)
	{
		printf("\n mutex init failed\n");
		return (EXIT_FAILURE);
	}
	for (int i = 0; i < NB_THREAD; i++)
	{
		err = pthread_create(&threads[i], NULL, job, &md);
		if (err != 0)
		{
			printf("Echec de la création du thread: [%s]", strerror(err));
			break ;
		}
		printf("Création du thread numéro %ld\n", threads[i]);
	}
	for (int i = 0; i < NB_THREAD; i++)
	{
		pthread_join(threads[i], NULL);
	}
	pthread_mutex_destroy(&md.mutex);
	return (EXIT_SUCCESS);
}
