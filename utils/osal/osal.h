/**
 * @file osal.h
 * @author Brad Powell
 * @date 18 Mar 2024
 * @brief File containing definitions for Operating System Abstraction Layer functions.
 *
 */

#ifndef OSAL_H_
#define OSAL_H_

#include <types.h>

typedef UINT08 osal_id_t;
typedef UINT16 osal_stack_t;
typedef UINT32 osal_task_t;
typedef void* (*osal_func)(void*);

typedef struct
{
    UINT08  priority;
    UINT08  subpriority;
}osal_priority_t;

sys_result_e osal_init(void);

sys_result_e osal_task_create(osal_id_t id, char *name, osal_stack_t stack, osal_priority_t priority, void *args);

sys_result_e osal_task_start(osal_id_t id);

sys_result_e osal_task_delete(osal_id_t id);

sys_result_e osal_task_suspend(osal_id_t id);

#endif // !OSAL_H_