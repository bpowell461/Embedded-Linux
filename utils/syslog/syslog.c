#include "syslog.h"
#include <stdio.h>
#include <time.h>
#include <string.h>
#include <stdint.h>
#include <stdarg.h>
#include "types.h"

/* Macros Defines */
#define USE_PRINTF 1

#define NULL_TERM_SIZE (1u)
#define MONTH_SIZE     (3u)
#define DAY_SIZE       (2u)
#define TIME_SIZE      (8u)

/* Type Defines */
typedef int (*printFunc_t)(const char* restrict, ...);
typedef int (*vprintFunc_t)(const char* restrict, __gnuc_va_list);

static char    *assignment_name;
static int      course_num;
static int      assignment_num;

static BOOL_T       isInitialized = DEF_FALSE;
static printFunc_t  _printFunc;
static vprintFunc_t _vprintFunc;


void syslog_init(char* assignment, const int courseNum, const int assignmentNum)
{
    if (!isInitialized)
    {
        assignment_name = assignment;
        course_num = courseNum;
        assignment_num = assignmentNum;

#if defined(USE_PRINTF) && USE_PRINTF
        _printFunc = &printf;
        _vprintFunc = &vprintf;
#else
        /* Implementation Specific */
        _printFunc = DEF_NULL_PTR;
        _vprintFunc = DEF_NULL_PTR;
#endif

        isInitialized = DEF_TRUE;
    }
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
        while (fgets(path, sizeof(path), fp) != NULL) {
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

        _printFunc("%s %s %s: [COURSE:%i][ASSIGNMENT:%i]", date, platform, assignment_name, course_num, assignment_num);

        va_list argp;
        va_start(argp, msg);
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
