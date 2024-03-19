#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <sched.h>
#include <time.h>
#include <stdint.h>
#include "syslog.h"

#define NUM_THREADS 128

#define ASSIGNMENT 3
#define COURSE 1
#define NAME "pthread"

typedef struct
{
    uint8_t threadIdx;
    uint8_t threadPriority;
    uint8_t threadCoreAffinity;
} threadargs_t;

typedef struct
{
    pthread_t thread;
    threadargs_t params;
}thread_handle_t;


// POSIX thread declarations and scheduling attributes
//
thread_handle_t threads[NUM_THREADS];

static pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;


// Unsafe global
int gsum = 0;

void* incThread(void* threadp)
{
    threadargs_t* threadParams = (threadargs_t*)threadp;

    gsum = gsum + 1;

    /* Mutex lock is ONLY for uninterrupted trace statements */
    pthread_mutex_lock(&mutex);
    SYSLOG_TRACE("Thread idx=%i, sum[1...%i]=%i", threadParams->threadIdx, threadParams->threadIdx, gsum);
    pthread_mutex_unlock(&mutex);

    pthread_exit(DEF_NULL_PTR);
}

int main(int argc, char* argv[])
{
    if (SYS_SUCCESS != syslog_init(NAME, COURSE, ASSIGNMENT))
        return SYS_FAILURE;

    syslog_printheader();

    int i = 0;

    for (i = 0; i < NUM_THREADS; i++)
    {
        threads[i].params.threadIdx = i+1;
        pthread_create(&threads[i].thread, DEF_NULL_PTR, incThread, (void*)&(threads[i].params));
    }

    for (i = 0; i < NUM_THREADS; i++)
        pthread_join(threads[i].thread, DEF_NULL_PTR);

    (void) syslog_close();

    return SYS_SUCCESS;
}
