/**
 * @file syslog.h
 * @author Brad Powell
 * @date 16 Mar 2024
 * @brief File containing definitions for system log functions.
 *
 */

#ifndef SYS_LOG_H_
#define SYS_LOG_H_

#include "types.h"

#if defined(BEAGLEBONE)
#define PLATFORM "beaglebone"
#elif defined(RASPBERRYPI)
#define PLATFORM "raspberrypi"
#else
#error "No platform defined!"
#endif

#if defined(DEBUG)
#define SYSLOG_TRACE(...)  syslog_trace(__VA_ARGS__)
#else
#define SYSLOG_TRACE(...)
#endif

/** 
* Initializes the System Log module.
* @param assignment - assignment name
* @param courseNum - course number
* @param assignmentNum - assignment number
* @return Result of operation.
*/
SysResult_e syslog_init(char *assignment, const int courseNum, const int assignmentNum);

/**
* Prints the System Log Header.
* @param None
* @return None
*/
void syslog_printheader(void);

/**
* Prints the System Log Header.
* @param msg - string to be printed
* @param varargs - variable amount of args
* @return None
*/
void syslog_trace(char* msg, ...);



#endif //SYS_LOG_H_