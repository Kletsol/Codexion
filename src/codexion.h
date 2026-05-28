/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbonnet <lbonnet@student.42mulhouse.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/03 15:04:07 by lbonnet           #+#    #+#             */
/*   Updated: 2026/05/28 15:37:15 by lbonnet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CODEXION_H
# define CODEXION_H

# include <stdlib.h>
# include <stdint.h>
# include <stdbool.h>
# include <stdio.h>
# include <limits.h>
# include <pthread.h>
# include <string.h>
# include <unistd.h>
# include <sys/time.h>

typedef enum e_scheduler_type
{
	FIFO = 0,
	EDF = 1
}	t_enum_sched;

typedef struct s_sim	t_sim;

typedef struct s_dongle
{
	pthread_mutex_t	mutex;
	int				id;
	uint64_t		available_at;
}	t_dongle;

typedef struct s_coder
{
	int				id;
	pthread_t		thread;
	t_dongle		*left_dongle;
	t_dongle		*right_dongle;
	uint64_t		last_compile_start;
	int				nb_compiles;
	pthread_mutex_t	state_mutex;
	t_sim			*sim;
}	t_coder;

typedef struct s_sim
{
	int				nb_coders;
	uint64_t		time_to_burnout;
	uint64_t		time_to_compile;
	uint64_t		time_to_debug;
	uint64_t		time_to_refactor;
	int				nb_compiles;
	uint64_t		cooldown;
	t_enum_sched	policy;
	bool			stop;
	uint64_t		start_time;
	pthread_mutex_t	stop_mutex;
	pthread_mutex_t	print_mutex;
	pthread_t		monitor;
	t_coder			*coders;
	t_dongle		*dongles;
}	t_sim;

uint64_t	ft_atou(const char *nptr, bool *error);
void		ft_bzero(void *s, size_t n);
void		*ft_calloc(size_t nmemb, size_t size);
bool		parser(char **av, t_sim *simulation);
bool		init_coders(t_sim *sim);
static void	destroy_dongles_mutexes(t_dongle *dongles, int count);
void		print_dongles(t_sim *sim);
uint64_t	get_time_ms(void);

#endif
