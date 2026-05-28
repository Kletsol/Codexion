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

typedef enum e_scheduler_type
{
	FIFO = 0,
	EDF = 1
}	t_enum_sched;

typedef struct s_coder
{
	int	nb_compiles;
}	t_coder;

typedef struct s_sim
{
	t_coder			*list_coders;
	int				nb_coders;
	uint64_t		time_to_burnout;
	uint64_t		time_to_compile;
	uint64_t		time_to_debug;
	uint64_t		time_to_refactor;
	int				nb_compiles;
	uint64_t		cooldown;
	t_enum_sched	policy;
}	t_sim;

uint64_t	ft_atou(const char *nptr, bool *error);
void		ft_bzero(void *s, size_t n);
void		*ft_calloc(size_t nmemb, size_t size);
bool		parser(char **av, t_sim *simulation);
bool		init_coders(t_sim *sim);

#endif
'
Maintenant, je ne vois pas bien par ou commencer et comment structurer le tout...
