#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <sched.h>
#include <time.h>
#include "syslog.h"

#define NUM_THREADS 1
#define ASSIGNMENT 1
#define COURSE 1

typedef struct
{
    char* threadMsg;
} threadParams_t;


// POSIX thread declarations and scheduling attributes
//
pthread_t threads[NUM_THREADS];
threadParams_t threadParams[NUM_THREADS];


void* counterThread(void* threadp)
{
    threadParams_t* threadParams = (threadParams_t*)threadp;

    SYSLOG_TRACE(threadParams->threadMsg);

    pthread_exit(NULL);
}


int main(int argc, char* argv[])
{
    syslog_init("pthread", COURSE, ASSIGNMENT);

    syslog_printheader();

    int i;

    SYSLOG_TRACE("Hello World from Main!");

    for (i = 0; i < NUM_THREADS; i++)
    {
        threadParams[i].threadMsg = "Hello World from Thread!";

        pthread_create(&threads[i],   // pointer to thread descriptor
            (void*)0,     // use default attributes
            counterThread, // thread function entry point
            (void*)&(threadParams[i]) // parameters to pass in
        );

    }

    for (i = 0; i < NUM_THREADS; i++)
        pthread_join(threads[i], NULL);


    return 0;
}