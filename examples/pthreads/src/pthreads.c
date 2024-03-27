#define _GNU_SOURCE
#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <sched.h>
#include <time.h>
#include <stdint.h>
#include <unistd.h>
#include "syslog.h"
#include "types.h"

#define NUM_THREADS 128

#define ASSIGNMENT 6
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
thread_handle_t startThread;
static pthread_attr_t attr;
static struct sched_param param;
static cpu_set_t cpuidx;


// Unsafe global
int gsum = 0;

void* incThread(void* threadp)
{
    threadargs_t* threadParams = (threadargs_t*)threadp;

    gsum = gsum + 1;

    /* Mutex lock is ONLY for uninterrupted trace statements */
    SYSLOG_TRACE("Thread idx=%i, sum[1...%i]=%i", threadParams->threadIdx, threadParams->threadIdx, gsum);

    pthread_exit(DEF_NULL_PTR);
}

void* bootstrapThread(void* threadp)
{
    int i = 0;

    for (i = 0; i < NUM_THREADS; i++)
    {
        threads[i].params.threadIdx = (UINT08)(i + 1);
        pthread_create(&threads[i].thread, &attr, incThread, (void*)&(threads[i].params));
    }

    for (i = 0; i < NUM_THREADS; i++)
        pthread_join(threads[i].thread, DEF_NULL_PTR);

    pthread_attr_destroy(&attr);

    pthread_exit(DEF_NULL_PTR);
}

int main(int argc, char* argv[])
{
    syslog_init(NAME, COURSE, ASSIGNMENT);

    syslog_printheader();
     
    /* Scheduler Setup */
    pthread_attr_init(&attr);
    pthread_attr_setinheritsched(&attr, PTHREAD_EXPLICIT_SCHED);
    pthread_attr_setschedpolicy(&attr, SCHED_FIFO);

    CPU_ZERO(&cpuidx);
    CPU_SET(0, &cpuidx);
    pthread_attr_setaffinity_np(&attr, sizeof(cpu_set_t), &cpuidx);

    param.sched_priority = 50;

    sched_setscheduler(getpid(), SCHED_FIFO, &param);
    pthread_attr_setschedparam(&attr, &param);

    pthread_create(&startThread.thread, &attr, bootstrapThread, DEF_NULL_PTR);

    pthread_join(startThread.thread, DEF_NULL_PTR);

    return SYS_SUCCESS;
}
