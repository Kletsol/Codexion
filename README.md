*This project has been created as part of the 42 curriculum by lbonnet*

# Codexion 🖇️

<span style="color:turquoise">

## 📝 Description
</span>


<span style="color:turquoise">

## 🖥️ Instructions
</span>


<span style="color:lightblue">

### ⤵️ Input
</span>


<span style="color:lightblue">

### ⤴️ Output
</span>


<span style="color:turquoise">

## 📚 Resources
</span>


<span style="color:turquoise">

## 🚀 Additional sections
</span>

### -> Blocking cases handled

### -> Thread synchronization mechanisms



-g3 -fsanitize=thread


J'ai un projet en c dont le but est de travailler sur les pthread et tout ce qui va avec (mutex...). Le sujet, en resume, est le suivant:
'Codexion is a concurrent programming project implementing a variation of the classic Dining Philosophers problem, adapted to a coding environment.

N coders sit in a circle around a shared Quantum Compiler. Each coder alternates between three activities: compiling, debugging, and refactoring. Compiling requires holding two USB dongles simultaneously (left and right). There are as many dongles as coders, one between each adjacent pair.

A coder burns out if they fail to start a new compilation within time_to_burnout milliseconds since their last compile (or the start of the simulation). The goal is to keep every coder alive long enough for all of them to complete the required number of compilations.

Each coder runs as an independent POSIX thread. A dedicated monitor thread polls the simulation state every 100 µs and detects burnout within the 10 ms precision constraint imposed by the subject.'

J'ai fait le parsing des arguments acceptes par le programme, qui sont les suivants:
'
number_of_coders: The number of coders and also the number of dongles.
◦ time_to_burnout (in milliseconds): If a coder did not start compiling within
time_to_burnout milliseconds since the beginning of their last compile or the
beginning of the simulation, they burn out.
◦ time_to_compile (in milliseconds): The time it takes for a coder to compile.
During that time, they must hold two dongles.
◦ time_to_debug (in milliseconds): The time a coder will spend debugging.
◦ time_to_refactor (in milliseconds): The time a coder will spend refactoring.
After completing the refactoring phase, the coder will immediately attempt to
acquire dongles and start compiling again.
◦ number_of_compiles_required: If all coders have compiled at least this
many times, the simulation stops. Otherwise, it stops when a coder burns
out.
◦ dongle_cooldown (in milliseconds): After being released, a dongle is unavail-
able until its cooldown has passed.
◦ scheduler: The arbitration policy used by dongles to decide who gets them
when multiple coders request them.
The value must be exactly one of: fifo or edf.
fifo means First In, First Out: the dongle is granted to the coder whose
request arrived first.
edf means Earliest Deadline First with deadline = last_compile_start +
time_to_burnout
'

J'ai aussi commence a ecrire le fichier .h, qui ressemble pour le moment a ceci:
'
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

typedef struct s_heap
{
	int			coder_id;
	uint64_t	request_time;
	uint64_t	deadline;
}	t_heap;

typedef struct s_dongle
{
	pthread_mutex_t	mutex;
	pthread_cond_t	cond;
	int				id;
	bool			available;
	uint64_t		available_at;
	t_heap			*queue;
	int				queue_size;
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
void		set_stop(t_sim *sim, bool value);
uint64_t	get_time_ms(void);
void		smart_sleep(uint64_t duration, t_sim *sim);
bool		print_error(char *str);
bool		simulation_stopped(t_sim *sim);
uint64_t	elapsed_time(t_sim *sim);
void		print_status(t_coder *coder, char *str);

#endif
'
J'ai aussi debute l'initialisation, avec les fonctions suivantes :
'
bool	init_dongles(t_sim *sim)
{
	int	i;

	sim->dongles = ft_calloc(sim->nb_coders, sizeof(t_dongle));
	if (!sim->dongles)
		return (false);
	i = 0;
	while (i < sim->nb_coders)
	{
		sim->dongles[i].id = i;
		sim->dongles[i].available_at = 0;
		if (pthread_mutex_init(&sim->dongles[i].mutex, NULL) != 0)
		{
			destroy_dongles_mutexes(sim->dongles, i);
			free(sim->dongles);
			return (false);
		}
		i++;
	}
	return (true);
}

bool	init_coders(t_sim *sim)
{
	int	i;

	sim->coders = ft_calloc(sim->nb_coders, sizeof(t_coder));
	if (!sim->coders)
		return (false);
	i = 0;
	while (i < sim->nb_coders)
	{
		sim->coders[i].id = i + 1;
		sim->coders[i].sim = sim;
		sim->coders[i].last_compile_start = get_time_ms();
		sim->coders[i].left_dongle = &sim->dongles[i];
		sim->coders[i].right_dongle = &sim->dongles[(i + 1) % sim->nb_coders];
		pthread_mutex_init(&sim->coders[i].state_mutex, NULL);
		i++;
	}
	return (true);
}
'
