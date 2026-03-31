#include "userlib/userlib.h"
#define COUNT_TO 1000000
#define NOISE_INTERVAL 1000
#define NCOUNTER 10

typedef void (*counter_func)();


// Busy counter
void busy_count();

// Noisy counter
void noisy_count();

// Less noisy counter
void less_noisy_count();

// create the counters
void create_counters(int *pids, counter_func func);

// wait for an array of counters to finish
int wait_counters(int *pids, const char* label);


int main()
{
    int busy_pids[NCOUNTER];
    int noisy_pids[NCOUNTER];
    int less_noisy_pids[NCOUNTER];
    int busy_done = 0;
    int noisy_done = 0;
    int less_noisy_done = 0;
    
 
    // start the counters
    create_counters(noisy_pids, noisy_count);
    create_counters(busy_pids, busy_count);
    create_counters(less_noisy_pids, less_noisy_count);

    // wait for them to complete
    while(!noisy_done || !busy_done || !less_noisy_done) {
        if(!noisy_done) { noisy_done=wait_counters(noisy_pids, "noisy"); }
        if(!busy_done) { busy_done=wait_counters(busy_pids, "busy"); }
        if(!less_noisy_done) { less_noisy_done=wait_counters(less_noisy_pids, "less noisy"); }
    }

    printf("All counters done.\n");

    return 0;
}

// Busy counter
void busy_count()
{
    int pid = getpid();
    printf("%d: Busy Counter Starting\n", pid);
    
    for(int i=0; i<COUNT_TO; i++) {
        // do nothing
    }
    printf("%d: Busy Counter Done\n", pid);
}

// Noisy counter
void noisy_count()
{
    int pid = getpid();
    printf("%d: Noisy Counter Starting\n", pid);
    for(int i=0; i<COUNT_TO; i++) {
        if(i%NOISE_INTERVAL == 0) {
            printf("%d:\t%d\n", pid, i);
        }
    }
    printf("%d: Noisy Counter Done\n", pid);
}


// Less noisy counter
void less_noisy_count()
{
    int pid = getpid();
    printf("%d: Less Noisy Counter Starting\n", pid);
    for(int i=0; i<COUNT_TO; i++) {
        if(i%(NOISE_INTERVAL*10) == 0) {
            printf("%d:\t%d\n", pid, i);
        }
    }
    printf("%d: Less Noisy Counter Done\n", pid);
}


void create_counters(int *pids, counter_func func)
{
    int pid;
    // start the busy counters
    for(int i=0; i<NCOUNTER; i++) {
        pid = clone();
        if(!pid) {
            func();
            terminate(getpid());
        }
        pids[i] = pid;
    }
}


int wait_counters(int *pids, const char* label) 
{
    int done = 1;

    for(int i=0; i<NCOUNTER; i++) {
        if(pids[i] == -1) continue;

        if(status(pids[i]) > 0) {
            done = 0;
        } else {
            pids[i] = -1;
        }
    }

    if(done) {
        printf("All %s counters are done\n", label);
    }

    return done;
}
