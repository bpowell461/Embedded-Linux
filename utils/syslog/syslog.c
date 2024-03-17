/**
 * @file syslog.c
 * @author Brad Powell
 * @date 16 Mar 2024
 * @brief File containing implementation for system log functions.
 *
 */

#include "syslog.h"
#include <stdio.h>
#include <time.h>
#include <string.h>
#include <stdint.h>
#include <stdarg.h>

/* Macros Defines */
#define USE_PRINTF 1

#define NULL_TERM_SIZE (1u)
#define MONTH_SIZE     (3u)
#define DAY_SIZE       (2u)
#define TIME_SIZE      (8u)

/* Types */
typedef int (*printFunc_t)(const char* restrict, ...);
typedef int (*vprintFunc_t)(const char* restrict, __gnuc_va_list);

/* Static Members */
static char         *assignment_name;
static int          course_num;
static int          assignment_num;
static BOOL_T       isInitialized = DEF_FALSE;
static printFunc_t  _printFunc;
static vprintFunc_t _vprintFunc;


/* Function Implementations */

SysResult_e syslog_init(char* assignment, const int courseNum, const int assignmentNum)
{
    if (!isInitialized)
    {
        assignment_name = assignment;
        course_num = courseNum;
        assignment_num = assignmentNum;

        /* Hook for OS or HAL specific trace function */
#if defined(USE_PRINTF) && USE_PRINTF
        _printFunc = &printf;
        _vprintFunc = &vprintf;
#else
        _printFunc = DEF_NULL_PTR;
        _vprintFunc = DEF_NULL_PTR;
#endif

        isInitialized = DEF_TRUE;

        return SYS_SUCCESS;
    }

    return SYS_FAILURE;
}

void syslog_printheader(void)
{
    if (isInitialized)
    {
        /* Currently this is very Linux Specific */
        FILE* fp;
        char path[1035];

        /* Open the command for reading. */
        fp = popen("uname -a", "r");
        if (fp == NULL) {
            return;
        }

        /* Read the output a line at a time - output it. */
        while(fgets(path, sizeof(path), fp) != NULL) {
            SYSLOG_TRACE(path);
        }

        /* close */
        pclose(fp);
    }

}

void syslog_trace(char *msg, ...)
{
    if (isInitialized)
    {
        /* Getting date and time in format MM DD HH:MM:SS*/

        time_t t = time(NULL);
        struct tm *local = localtime(&t);
    
        char month[MONTH_SIZE + NULL_TERM_SIZE];
        char day[DAY_SIZE + NULL_TERM_SIZE];
        char ti [TIME_SIZE + NULL_TERM_SIZE];
    
        char date[MONTH_SIZE+DAY_SIZE+TIME_SIZE+NULL_TERM_SIZE + NULL_TERM_SIZE + NULL_TERM_SIZE + NULL_TERM_SIZE];
    
        const char *platform = PLATFORM;
    
        (void) strftime(month, sizeof(month), "%b", local);
        (void) strftime(day, sizeof(day), "%d", local);
        (void) strftime(ti, sizeof(ti), "%X", local);
    
        snprintf(date, sizeof(month) + sizeof(day) + sizeof(ti) + NULL_TERM_SIZE, "%s %s %s", month, day, ti);

        _printFunc("%s %s %s: [COURSE:%i][ASSIGNMENT:%i]: ", date, platform, assignment_name, course_num, assignment_num);

        /* Retrieving varargs list for use with printf */
        va_list argp;
        va_start(argp, msg);

        /* For now we are appending a newline character */
        if (msg[strlen(msg)] != '\n')
        {
            char formatted_msg[strlen(msg) + 1 + NULL_TERM_SIZE];
            snprintf(formatted_msg, sizeof(formatted_msg), "%s\n", msg);
            _vprintFunc(formatted_msg, argp);
        }
        else
        {
            _vprintFunc(msg, argp);
        }
        va_end(argp);
    }
    
}
