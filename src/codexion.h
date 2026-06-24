/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lbonnet <lbonnet@student.42mulhouse.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/03 15:04:07 by lbonnet           #+#    #+#             */
/*   Updated: 2026/06/24 15:03:35 by lbonnet          ###   ########.fr       */
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
# define ERROR_INT_TOO_LOW "Argument must be equal or greater than 1\n"
# define ERROR_OVERFLOW "Argument invalid, may cause integer overflow\n"
# define ERROR_NB_CODERS "Number of coders must be greater than 0\n"
# define ERROR_NB_COMPILES "Number of compiles must be greater than 0\n"
# define ERROR_POLICY "The chosen scheduler must be exactly fifo or edf\n"

# define COMPILE "\033[1;32mis compiling\033[0;0m"
# define DEBUG "\033[1;33mis debugging\033[0;0m"
# define REFACTOR "\033[1;34mis refactoring\033[0;0m"
# define BURNOUT "\033[1;31mburned out\033[0;0m"

typedef struct s_sim	t_sim;
typedef struct s_coder	t_coder;

typedef enum e_scheduler_type
{
	FIFO = 0,
	EDF = 1
}	t_enum_sched;

typedef struct s_to_clean
{
	int			nb_dongles;
	int			nb_coders;
	bool		print_mutex;
	bool		sim_mutex;
	bool		stop_mutex;
}	t_to_clean;

typedef struct s_dongle
{
	pthread_mutex_t	dongle;
	pthread_mutex_t	heap;
	pthread_mutex_t	state;
	uint64_t		last_use;
	t_coder			*waiters[2];
}	t_dongle;

typedef struct s_coder
{
	int				id;
	pthread_t		coder_thread;
	pthread_mutex_t	coder_mutex;
	t_dongle		*left;
	t_dongle		*right;
	t_sim			*sim;
	uint64_t		compile_start;
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
	bool			silence;
	struct timeval	start_time;
	pthread_mutex_t	sim_mutex;
	pthread_mutex_t	stop_mutex;
	pthread_mutex_t	print_mutex;
	pthread_t		monitor;
	t_coder			*coders;
	t_dongle		*dongles;
	int				finished_coders;
	t_to_clean		cleanup;
}	t_sim;

// utils
uint64_t		ft_atou(const char *nptr, bool *error);
void			swap(t_dongle *dongle);
uint64_t		get_time_ms(t_sim *sim);

//memory
void			ft_bzero(void *s, size_t n);
void			*ft_calloc(size_t nmemb, size_t size);

// parsing
bool			parser(char **av, t_sim *simulation);

// init
bool			init_dongles(t_sim *sim);
bool			init_coders(t_sim *sim);
bool			init_simulation(t_sim *sim);

// coders
void			*coder_routine(void *arg);
bool			compile(t_coder *coder);
bool			debug(t_coder *coder);
bool			refactor(t_coder *coder);

// dongles
void			lock_dongles(t_coder *coder, t_sim *sim);
void			unlock_dongles(t_coder *coder);

// cleanup
void			destroy_simulation(t_sim *sim);

// getters
bool			get_stop(t_sim *sim);
bool			get_dongle_state(t_dongle *dongle, t_sim *sim);

// setters
void			set_stop(t_sim *sim);

// errors
bool			print_error(char *str);

//simulation
void			simulation(t_sim *sim);

// logs
void			print_status(t_coder *coder, char *str, bool burnout);

// monitor
bool			check_burnout(t_sim *sim);
void			*monitor_routine(void *arg);

// heap
void			heap_push(t_coder *coder, t_dongle *dongle);
void			heap_pop(t_coder *coder, t_dongle *dongle);
int				heap_peek(t_dongle *dongle);
bool			scheduler(t_coder *coder, t_sim *sim);
void			resolve_edf(t_dongle *dongle, t_sim *sim);

#endif
