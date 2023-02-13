#define MIN_PRIORITY 1
#define MAX_PRIORITY 10

// add a task to the list 
void add(char *name, int priority, int burst, int tid);

// invoke the scheduler
void schedule();