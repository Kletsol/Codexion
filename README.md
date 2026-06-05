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

Quand un coder veut compiler, il :

	crée une requête
	↓
	s'insère dans la file gauche
	↓
	s'insère dans la file droite
	↓
	attend

Puis il est autorisé à compiler uniquement si :

	il est premier dans la file gauche
	ET
	il est premier dans la file droite
	ET
	les deux dongles sont disponibles
	ET
	les cooldowns sont terminés


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
Et quelques regles en plus :
'
The specific rules for the mandatory part are:
• Each coder must be represented by a thread (using pthread_create).
• There is one dongle between each pair of coders. Therefore, if there are several
coders, each coder has a dongle on their left side and a dongle on their right side.
If there is only one coder, there should be only one dongle on the table.
• To prevent coders from duplicating dongles, you must protect each dongle’s state
with a mutex (pthread_mutex_t). A condition variable (pthread_cond_t) may be
used to manage waiting queues.
• Dongle cooldown is mandatory: after a coder releases a dongle, the dongle
cannot be taken again until dongle_cooldown milliseconds have elapsed.
• Fair arbitration is mandatory: when multiple coders request the same dongle,
11
Codexion Master the race for resources before the deadline masters you
the dongle must grant access according to scheduler.
With fifo, serve requests in arrival order.
With edf, serve the coder with the earliest burnout deadline (i.e., last_compile_start
+ time_to_burnout).
• The program must guarantee liveness: coders should not starve under edf schedul-
ing, provided the parameters are feasible.
• A separate monitor thread must detect burnout precisely and stop the simulation.
The burnout log must be printed within 10 ms of the actual burnout time.
• Logging must be serialized so that two messages never interleave on a single line
(use a mutex to protect output).
• The simulation stops either when a coder burns out or when every coder has com-
piled at least number_of_compiles_required times.
• Your code must compile with -Wall -Wextra -Werror -pthread.
• You must implement a priority queue (heap) for FIFO/EDF scheduling (no stan-
dard library priority queue may be used).
• All memory must be properly allocated and freed (no memory leaks).
'
J'ai aussi redige le fichier .h, qui ressemble pour le moment a ceci:
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
static void	swap(t_request *a, t_request *b);
bool		request_priority(t_request *a, t_request *b, t_enum_sched policy);
bool		heap_push(t_heap *heap, t_request request, t_enum_sched policy);
t_request	heap_pop(t_heap *heap, t_enum_sched policy);
t_request	*heap_peek(t_heap *heap);

#endif
'
Toutes les fonctions mentionnees dans le fichier .h sont deja entierement ecrites.
J'ai egalement :
- Un main :
'
int	main(int ac, char **av)
{
	t_sim	simulation;

	if (ac != 9)
	{
		print_error(ERROR_MISSING_ARG);
		return (1);
	}
	else if (!parser(av, &simulation))
		return (1);
	else
	{
		printf("%s", "[Parsing succeeded !]\n\n");
		init_simulation(&simulation);
		destroy_simulation(&simulation);
		return (0);
	}
}
- Une initialisation :
'
bool	init_heap(t_dongle *dongle, t_sim *sim)
{
	dongle->waiters.capacity = sim->nb_coders;
	dongle->waiters.size = 0;
	dongle->waiters.data = ft_calloc(sim->nb_coders, sizeof(t_request));
	if (!dongle->waiters.data)
		return (false);
	return (true);
}

bool	init_dongles(t_sim *sim)
{
	int	i;

	sim->dongles = ft_calloc(sim->nb_coders, sizeof(t_dongle));
	if (!sim->dongles)
		return (false);
	i = -1;
	while (++i < sim->nb_coders)
	{
		sim->dongles[i].id = i;
		sim->dongles[i].available = true;
		sim->dongles[i].available_at = 0;
		if ((!init_heap(&sim->dongles[i], sim))
			|| pthread_mutex_init(&sim->dongles[i].mutex, NULL) != 0)
		{
			destroy_dongles(sim, i);
			return (false);
		}
		if (pthread_cond_init(&sim->dongles[i].cond, NULL) != 0)
		{
			pthread_mutex_destroy(&sim->dongles[i].mutex);
			destroy_dongles(sim, i);
			return (false);
		}
		printf("Dongle %d initialized\n", sim->dongles[i].id);
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
		sim->coders[i].last_compile_start = sim->start_time;
		sim->coders[i].left_dongle = &sim->dongles[i];
		sim->coders[i].right_dongle = &sim->dongles[(i + 1) % sim->nb_coders];
		if (pthread_mutex_init(&sim->coders[i].state_mutex, NULL) != 0)
		{
			destroy_coders(sim, i);
			return (false);
		}
		printf("Coder %d initialized\n", sim->coders[i].id);
		i++;
	}
	return (true);
}

bool	init_simulation(t_sim *sim)
{
	sim->start_time = get_time_ms();
	sim->stop = false;
	if (pthread_mutex_init(&sim->stop_mutex, NULL) != 0)
		return (false);
	if (pthread_mutex_init(&sim->print_mutex, NULL) != 0)
	{
		pthread_mutex_destroy(&sim->stop_mutex);
		return (false);
	}
	if (!init_dongles(sim))
	{
		destroy_global_mutexes(sim);
		return (false);
	}
	if (!init_coders(sim))
	{
		destroy_dongles(sim, sim->nb_coders);
		destroy_global_mutexes(sim);
		return (false);
	}
	return (true);
}
'
- Un monitor :
'
void	check_burnout(t_sim *sim)
{
	int			i;
	uint64_t	elapsed;
	uint64_t	time;

	i = 0;
	while (i < sim->nb_coders)
	{
		pthread_mutex_lock(&sim->coders[i].state_mutex);
		elapsed = get_time_ms() - sim->coders[i].last_compile_start;
		if (elapsed >= sim->time_to_burnout)
		{
			time = elapsed_time(sim->coders[i].sim);
			sim->coders[i].state = BURNED_OUT;
			set_stop(sim, true);
			pthread_mutex_unlock(&sim->coders[i].state_mutex);
			pthread_mutex_lock(&sim->print_mutex);
			printf("%lu %d burned out\n", time, sim->coders[i].id);
			pthread_mutex_unlock(&sim->print_mutex);
			return ;
		}
		pthread_mutex_unlock(&sim->coders[i].state_mutex);
		i++;
	}
}

bool	check_completion(t_sim *sim)
{
	int		i;

	i = 0;
	while (i < sim->nb_coders)
	{
		pthread_mutex_lock(&sim->coders[i].state_mutex);
		if (sim->coders[i].nb_compiles < sim->nb_compiles)
		{
			pthread_mutex_unlock(&sim->coders[i].state_mutex);
			return (false);
		}
		pthread_mutex_unlock(&sim->coders[i].state_mutex);
		i++;
	}
	return (true);
}

void	*monitor_routine(void *arg)
{
	t_sim	*sim;

	sim = (t_sim *)arg;
	while (!simulation_stopped(sim))
	{
		check_burnout(sim);
		if (simulation_stopped(sim))
			break ;
		if (check_completion(sim))
		{
			set_stop(sim, true);
			break ;
		}
		usleep(100);
	}
	return (NULL);
}
'
Et les fonctions de la heap :
'
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
	t_request	smallest;

	i = 0;
	if (heap->size == 0)
		return ({0});
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
'
