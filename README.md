# codexion
A multithreaded C project exploring concurrency, synchronization, mutexes, condition variables, and time-based coordination.


## ./codexion 3 6 77 88 11 2 3 edf  
## ./codexion 4 800 200 100 100 3 100 fifo


## ./codexion 4 800 200 100 100 3 100 fifo
## cc -fsanitize=thread

## valgrind --tool=helgrind ./codexion 4 800 200 100 100 3 100 fifo
## -Wall -Wextra -Werror -pthread -fsanitize=thread