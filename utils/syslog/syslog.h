#ifndef SYS_LOG_H_
#define SYS_LOG_H_

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

void syslog_init(char *assignment, const int courseNum, const int assignmentNum);

void syslog_printheader(void);

void syslog_trace(char* msg, ...);

#endif //SYS_LOG_H_