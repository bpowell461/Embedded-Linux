/**
 * @file types.h
 * @author Brad Powell
 * @date 16 Mar 2024
 * @brief File containing common types and macros.
 *
 */
#ifndef TYPES_H_
#define TYPES_H_

#include <stdint.h>
#include <stddef.h>

#define DEF_TRUE        (1u == 1u)
#define DEF_FALSE       (!DEF_TRUE)
#define DEF_NULL_PTR    (NULL)

typedef uint8_t     BOOL_T;
typedef uint8_t     UINT08;
typedef uint16_t    UINT16;
typedef uint32_t    UINT32;
typedef int8_t      INT08;
typedef int16_t     INT16;
typedef int32_t     INT32;

typedef enum
{
    SYS_SUCCESS,
    SYS_FAILURE,
    SYS_IGNORE,
    SYS_COUNT
}sys_result_e;

#endif // !TYPES_H_
