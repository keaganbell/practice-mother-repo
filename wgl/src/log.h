#pragma once

#include <stdio.h>
#include <string.h>

#define __FILENAME__ (strrchr(__FILE__, '\\') ? strrchr(__FILE__, '\\') + 1 : __FILE__)

enum log_level {
    LOG_TRACE,
    LOG_DEBUG,
    LOG_INFO,
    LOG_WARN,
    LOG_ERROR,
    LOG_FATAL
};

static void _PlatformDebugPrint(log_level Level, const char *Filename, u32 Line, const char *Message, ...);

#define LTRACE(msg, ...) _PlatformDebugPrint(LOG_TRACE, __FILENAME__, __LINE__, msg, ##__VA_ARGS__)
#define LDEBUG(msg, ...) _PlatformDebugPrint(LOG_DEBUG, __FILENAME__, __LINE__, msg, ##__VA_ARGS__)
#define  LINFO(msg, ...) _PlatformDebugPrint(LOG_INFO,  __FILENAME__, __LINE__, msg, ##__VA_ARGS__)
#define  LWARN(msg, ...) _PlatformDebugPrint(LOG_WARN,  __FILENAME__, __LINE__, msg, ##__VA_ARGS__)
#define LERROR(msg, ...) _PlatformDebugPrint(LOG_ERROR, __FILENAME__, __LINE__, msg, ##__VA_ARGS__)
#define LFATAL(msg, ...) _PlatformDebugPrint(LOG_FATAL, __FILENAME__, __LINE__, msg, ##__VA_ARGS__)
