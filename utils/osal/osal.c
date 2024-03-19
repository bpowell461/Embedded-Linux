/**
 * @file osal.c
 * @author Brad Powell
 * @date 18 Mar 2024
 * @brief File containing definitions for Operating System Abstraction Layer functions.
 *
 */
#include "osal.h"
#include "pthread.h"

#define MAX_TASKS 128

typedef struct
{
    osal_id_t       task_id;
    char*           task_name;
    osal_stack_t    task_stack;
    osal_priority_t task_priority;
    osal_func       task_func;
    void*           task_args;
    osal_task_t*    task_handle;
}osal_task_tcb_t;

static osal_task_tcb_t osal_task_tcb[MAX_TASKS];

static UINT08 task_idx = 0;

sys_result_e osal_init(void)
{

}

sys_result_e osal_task_create(osal_id_t id, char* name, osal_stack_t stack, osal_priority_t priority, void* args)
{
    int res;
    pthread_attr_t attr;

    res = pthread_attr_init(&attr);
    if (res)
    {
        return SYS_FAILURE;
    }

    struct sched_param sp;
    res = pthread_attr_getschedparam(&attr, &sp);


}

sys_result_e osal_task_start(osal_id_t id)
{

}

sys_result_e osal_task_delete(osal_id_t id)
{

}

sys_result_e osal_task_suspend(osal_id_t id)
{

}