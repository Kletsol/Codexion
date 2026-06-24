*This project has been created as part of the 42 curriculum by lbonnet*

# Codexion 🖇️

<span style="color:turquoise">

## 📝 Description
</span>

Codexion is a project written in C and based on the use of threads, in the<br>
form of a dining philosophers problem with a few subtleties.

Instead of philosophers and meals, we are dealing with coders and dongles,<br>
spread out in a circle, one dongle between each coder. Each coder can<br>
therefore take two dongles, one on his left and one on his right and, once<br>
he holds both of them, perform the following actions :

- Compile
- Debug
- Refactor

When a coder finishes compiling, he puts both dongles back on the table so<br>
his neighbors can use them. Then, he starts debugging. Once debugging is<br>
done, he starts refactoring. Finally, he can try to compile again.

If multiple coders request the same dongle, the dongle grants access<br>
according to the selected scheduler :

- 'fifo' serves requests in arrival order : First In, First Out.
- 'edf' serves the coder with the Earliest Deadline First.

If a coder did not start compiling within a given time since the beginning<br>
of their last compile or the beginning of the simulation, they burn out<br>
and the simulation stops immediately.

Otherwise, the simulation goes on until every coder compiled at least a<br>
given nuber of times.

<span style="color:turquoise">

## 🖥️ Instructions
</span>

<span style="color:lightblue">

### ⤵️ Input
</span>

As you may have seen in the description, this program take a little bit <br>
more arguments than a classic philosophers :

|Arg|Type|Description|
|---|---|---|
|number_of_coders|Int|The number of coders and also the number of dongles|
|time_to_burnout|uint64_t|The time until a coder that doesn't compile burns out|
|time_to_compile|uint64_t|The time it take for a coder to compile|
|time_to_debug|uint64_t|The time a coder will spend debugging|
|time_to_refactor|uint64_t|The time a coder will spend refactoring|
|number_of_compiles|int|The number of compiles required for each coder|
|dongle_cooldown|uint64_t|The duration for which a dongle is unusable after compilation|
|scheduler|str|The chosen arbitration policy used by dongles : "fifo" or "edf"|

Execute the command :

	make

to compile, then follow this model :

	./codexion <number_of_coders> <time_to_burnout> <time_to_compile> <time_to_debug> <time_to_refactor> <number_of_compiles> <dongle_cooldown> <scheduler>


<span style="color:lightblue">

### ⤴️ Output
</span>

The output appears directly on the terminal after execution, and depends on<br>
the arguments given. Each line of the output has a very precise form :

	<time> <coder_id> <action>

With :

- time : the time elapsed since start, in milliseconds
- coder_id : a number between 1 and number_of_coders
- action : the action performed by the coder

Example :

	400 4 has taken a dongle
	400 4 has taken a dongle
	400 4 is compiling
	400 5 is refactoring

These lines can then be interpreted with the arguments given at the start.

<span style="color:turquoise">

## 📚 Resources
</span>

Here are some useful links I used to better understand how threads work and<br>
how to use it :

https://tala-informatique.fr/index.php?title=C_pthread

https://github.com/KeroBeros68/Obsidian-vault/tree/main/c/pthread

https://franckh.developpez.com/tutoriels/posix/pthreads/

AI was used mainly for debugging when I couldn't figure out myself where<br>
the problem was. This mainly concerned data race issues, as helgrind reports<br>
both EXISTING and POTENTIAL data races.

<span style="color:turquoise">

## 🚀 Additional sections
</span>

### -> Blocking cases handled

First, the project you are currently reviewing is the fifth version of Codexion<br>
I had to create in order to finally get a working program in all aspects.<br>
While working on the previous versions, I encountered several issues such as :

- Helgrind warnings (not errors, but still),
- Unexpected starvation,
- Waiting conditions (pthread_cond_wait or timedwait) not resolving,
- Structures too dense, too many mutexes everywhere, difficult to debug.

This fifth version is intended to be simpler, with less intricate structures,<br>
threads and mutexes.

On the other hand, I've never had too many problems with Valgrind and memory<br>
leaks, as I designed the initialization from the start in such a way that I<br>
could clean everything up properly at the end.

Note that, having struggled with that for three whole versions, this version<br>
does not use pthread_cond in any way, and simply relies on while loops.

### -> Thread synchronization mechanisms

My implementation only uses threads in two ways : one for each coder, and one<br>
for the monitor.

Each coder also has an associated mutex that's used every time the program<br>
needs to access data from the coder, to prevent data races between threads.

Each dongle has three mutexes protecting the access to the dongle itself, to<br>
its heap, and to it's state (either taken or available).

Finally, the simulation itself has three mutexes protecting simulation shutdown,<br>
printing function and access to any process likely to cause a shutdown like<br>
burnout or finished coders.

On top of that, the monitor manages the whole simulation, checking regularly<br>
for burnout and verifying the number of finished coders, shutting down the<br>
simulation if either of these occurs.

The correct usage of dedicated functions with dedicated mutexes (e.g. getters<br>
and setters) prevents any data race to happen, as well as other issues like<br>
messages interleaving on a single line.
