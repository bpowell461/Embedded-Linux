#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <sched.h>
#include <time.h>
#include "syslog.h"

#define NUM_THREADS 128

#define ASSIGNMENT 2
#define COURSE 1
#define NAME "pthread"

#define COUNT  1000

typedef struct
{
    int threadIdx;
} threadParams_t;


// POSIX thread declarations and scheduling attributes
//
pthread_t threads[NUM_THREADS];
threadParams_t threadParams[NUM_THREADS];

static pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;


// Unsafe global
int gsum = 0;

void* incThread(void* threadp)
{
    int i;
    threadParams_t* threadParams = (threadParams_t*)threadp;

    for (i = 0; i < COUNT; i++)
    {
        gsum = gsum + i;

        /* Mutex lock is ONLY for uninterrupted trace statements */
        pthread_mutex_lock(&mutex);
        SYSLOG_TRACE("Thread idx=%i, sum[1...%i]=%i", threadParams->threadIdx, threadParams->threadIdx, gsum);
        pthread_mutex_unlock(&mutex);
    }

    pthread_exit(DEF_NULL_PTR);
}

void* decThread(void* threadp)
{
    int i;
    threadParams_t* threadParams = (threadParams_t*)threadp;

    for (i = 0; i < COUNT; i++)
    {
        gsum = gsum - i;

        /* Mutex lock is ONLY for uninterrupted trace statements */
        pthread_mutex_lock(&mutex);
        SYSLOG_TRACE("Thread idx=%i, sum[1...%i]=%i", threadParams->threadIdx, threadParams->threadIdx, gsum);
        pthread_mutex_unlock(&mutex);
    }

    pthread_exit(DEF_NULL_PTR);
}

int main(int argc, char* argv[])
{
    syslog_init(NAME, COURSE, ASSIGNMENT);
    syslog_printheader();

    int rc;
    int i = 0;

    for (i = 0; i < NUM_THREADS; i++)
    {
        threadParams[i].threadIdx = i+1;
        pthread_create(&threads[i], DEF_NULL_PTR, (i & 1) ? incThread : decThread, (void*)&(threadParams[i]));
    }

    for (i = 0; i < NUM_THREADS; i++)
        pthread_join(threads[i], DEF_NULL_PTR);

    return SYS_SUCCESS;
}
