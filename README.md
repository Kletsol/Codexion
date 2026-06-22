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


J'ai un projet en c dont le but est de travailler sur les pthread et tout ce qui va avec (mutex...)
Sujet resume :
'
Codexion is a concurrent programming project implementing a variation of the classic Dining Philosophers problem, adapted to a coding environment.

N coders sit in a circle around a shared Quantum Compiler. Each coder alternates between three activities: compiling, debugging, and refactoring. Compiling requires holding two USB dongles simultaneously (left and right). There are as many dongles as coders, one between each adjacent pair.

A coder burns out if they fail to start a new compilation within time_to_burnout milliseconds since their last compile (or the start of the simulation). The goal is to keep every coder alive long enough for all of them to complete the required number of compilations.

Each coder runs as an independent POSIX thread. A dedicated monitor thread polls the simulation state every 100 µs and detects burnout within the 10 ms precision constraint imposed by the subject.'

Arguments acceptes par le programme :
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
Quelques regles en plus :
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
Fichier .h actuel :
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
typedef struct s_coder	t_coder;

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

typedef struct s_to_clean
{
	int			nb_dongles;
	int			nb_coders;
}	t_to_clean;

typedef struct s_request
{
	int			coder_id;
	uint64_t	priority;
}	t_request;

typedef struct s_heap
{
	t_request	*data;
	int			size;
	int			capacity;
}	t_heap;

typedef struct s_dongle
{
	int				id;
	pthread_mutex_t	mutex;
	bool			taken;
	uint64_t		end_cooldown;
	t_heap			waiters;
}	t_dongle;

typedef struct s_coder
{
	int				id;
	pthread_t		cod_thread;
	pthread_mutex_t	death_mutex;
	pthread_mutex_t	comp_mutex;
	t_dongle		*left;
	t_dongle		*right;
	t_sim			*sim;
	uint64_t		compile_start;
	int				nb_compiles;
	uint64_t		deadline;
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
	pthread_mutex_t	sim_mutex;
	pthread_mutex_t	stop_mutex;
	pthread_mutex_t	print_mutex;
	pthread_mutex_t	sched_mutex;
	pthread_cond_t	sched_cond;
	pthread_t		monitor;
	t_coder			*coders;
	t_dongle		*dongles;
	int				finished_coders;
	t_to_clean		cleanup;
}	t_sim;

// utils
uint64_t		ft_atou(const char *nptr, bool *error);
uint64_t		get_time_ms(void);
void			smart_sleep(uint64_t duration, t_sim *sim);
uint64_t		get_next_seq(t_sim *sim);
struct timespec	compute_next_wake(t_dongle *d);

// time
long long		get_time_in_usec(void);
void			psleep(long long usec);

//memory
void			ft_bzero(void *s, size_t n);
void			*ft_calloc(size_t nmemb, size_t size);

// parsing
bool			parser(char **av, t_sim *simulation);

// init
bool			init_dongles(t_sim *sim);
void			swap_hands(t_coder *coder);
bool			init_coders(t_sim *sim);
bool			init_simulation(t_sim *sim);

// coders
void			*coder_routine(void *arg);
bool			debug(t_coder *coder);
bool			refactor(t_coder *coder);
bool			can_compile(t_coder *coder);

// dongles


// cleanup
void			join_coders(t_sim *sim);
void			destroy_simulation(t_sim *sim);

// debug
void			print_dongles(t_sim *sim);

// getters
bool			get_stop(t_sim *sim);
int				get_nb_compiles(t_coder *coder);
uint64_t		get_deadline(t_coder *coder);
int				get_finished_coders(t_sim *sim);
uint64_t		get_priority(t_coder *coder);

// setters
void			set_stop(t_sim *sim, bool value);;
void			set_deadline(t_coder *coder, uint64_t deadline);
void			set_nb_compiles(t_coder *coder, int value);
void			set_finished_coder(t_sim *sim, int value);

// errors
bool			print_error(char *str);

//simulation

void			simulation(t_sim *sim);

// logs
uint64_t		elapsed_time(t_sim *sim);
void			print_status(t_coder *coder, char *str);

// monitor
bool			check_burnout(t_sim *sim, uint64_t time);
void			*monitor_routine(void *arg);

// heap
// static void	swap(t_request *a, t_request *b);
void			heap_remove(t_heap *heap, int index);
void			heap_push(t_heap *heap, t_request request);
t_request		heap_pop(t_heap *heap);
t_request		heap_peek(t_heap *heap);
t_request		heap_pop_valid(t_heap *h);

#endif
'
Quelques precisions avant tout :
- C'est ma quatrieme version du code, il reste donc tres probablement des artefacts des precedentes implementations dans le .h et dans d'autres fichiers que je serais amene a montrer.

- Je gererai les exceptions et cas particuliers a la fin, donc inutile de venir avec des cas comme "si il n'y a un seul coder, etc."

- Si un coder burnout, la simulation s'arrete, donc inutile de retirer des requetes mortes de la heap

- Chaque heap est de capacite 2, car seuls les deux coders voisins du dongle peuvent le prendre

L'idee avec cette quatrieme version est de repartir sur une base saine apres bien trop d'elucubrations et de morceaux de code partis trop loin. On respecte donc les principes suivants :
- 1 mutex par dongle
- 1 heap par dongle
- 1 condition globale dans sim

J'ai deja reecrit, suivant ces principes :
- La routine coders
- Les fonctions des dongles
- L'initialisation
- Le nettoyage
- Une partie des fonctions de la heap, a verifier dans la nesure ou c4est l'une des parties les plus dangereuses du projet.
