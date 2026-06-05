/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbonnet <lbonnet@student.42mulhouse.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/03 15:04:07 by lbonnet           #+#    #+#             */
/*   Updated: 2026/06/05 10:57:02 by lbonnet          ###   ########.fr       */
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

# define ERROR_MISSING_ARG "Invalid number of arguments\n"
# define ERROR_NOT_POS_INT "Argument must be a positive int\n"
# define ERROR_OVERFLOW "Argument invalid, may cause integer overflow\n"
# define ERROR_NB_CODERS "Number of coders must be greater than 0\n"
# define ERROR_NB_COMPILES "Number of compiles must be greater than 0\n"
# define ERROR_POLICY "The chosen scheduler must be exactly fifo or edf\n"

typedef struct s_sim	t_sim;

typedef enum e_scheduler_type
{
	FIFO = 0,
	EDF = 1
}	t_enum_sched;

typedef enum e_state
{
	WAITING,
	COMPILING,
	DEBUGGING,
	REFACTORING,
	BURNED_OUT
}	t_state;

typedef struct s_request
{
	int			coder_id;
	uint64_t	request_time;
	uint64_t	deadline;
	uint64_t	seq;
}	t_request;

typedef struct s_heap
{
	t_request	*data;
	int			size;
	int			capacity;
}	t_heap;

typedef struct s_dongle
{
	pthread_mutex_t	mutex;
	pthread_cond_t	cond;
	int				id;
	bool			available;
	uint64_t		available_at;
	t_heap			waiters;
}	t_dongle;

typedef struct s_coder
{
	int				id;
	pthread_t		thread;
	t_dongle		*left_dongle;
	t_dongle		*right_dongle;
	pthread_mutex_t	state_mutex;
	t_sim			*sim;
	t_state			state;
	uint64_t		last_compile_start;
	int				nb_compiles;
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

// utils
uint64_t	ft_atou(const char *nptr, bool *error);
uint64_t	get_time_ms(void);
void		smart_sleep(uint64_t duration, t_sim *sim);

//memory
void		ft_bzero(void *s, size_t n);
void		*ft_calloc(size_t nmemb, size_t size);

// parsing
bool		parser(char **av, t_sim *simulation);

// init
bool		init_heap(t_dongle *dongle, t_sim *sim);
bool		init_coders(t_sim *sim);
bool		init_dongles(t_sim *sim);
bool		init_simulation(t_sim *sim);

// coders
void		*coder_routine(void *arg);
bool		start_coders(t_sim *sim);
void		wait_threads(t_sim *sim);

// cleanup
void		destroy_dongles(t_sim *sim, int count);
void		destroy_coders(t_sim *sim, int count);
void		destroy_global_mutexes(t_sim *sim);
void		destroy_simulation(t_sim *sim);

// debug
void		print_dongles(t_sim *sim);

// setters
void		set_stop(t_sim *sim, bool value);

// errors
bool		print_error(char *str);

//simulation
bool		simulation_stopped(t_sim *sim);

// logs
uint64_t	elapsed_time(t_sim *sim);
void		print_status(t_coder *coder, char *str);

// monitor
void		check_burnout(t_sim *sim);
bool		check_completion(t_sim *sim);
void		*monitor_routine(void *arg);

// heap
// static void	swap(t_request *a, t_request *b);
bool		request_priority(t_request *a, t_request *b, t_enum_sched policy);
bool		heap_push(t_heap *heap, t_request request, t_enum_sched policy);
t_request	heap_pop(t_heap *heap, t_enum_sched policy);
t_request	*heap_peek(t_heap *heap);

#endif
