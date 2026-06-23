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








Gerer le cas de compiles_required = 1;
