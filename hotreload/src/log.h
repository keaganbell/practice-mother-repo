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

#define PLATFORM_DEBUG_PRINT(name) void name(log_level Level, const char *Filename, u32 Line, const char *Message, ...);
typedef PLATFORM_DEBUG_PRINT(platform_debug_print);

#define LTRACE(_DebugPrint, msg, ...) _DebugPrint(LOG_TRACE, __FILENAME__, __LINE__, msg, ##__VA_ARGS__)
#define LDEBUG(_DebugPrint, msg, ...) _DebugPrint(LOG_DEBUG, __FILENAME__, __LINE__, msg, ##__VA_ARGS__)
#define  LINFO(_DebugPrint, msg, ...) _DebugPrint(LOG_INFO,  __FILENAME__, __LINE__, msg, ##__VA_ARGS__)
#define  LWARN(_DebugPrint, msg, ...) _DebugPrint(LOG_WARN,  __FILENAME__, __LINE__, msg, ##__VA_ARGS__)
#define LERROR(_DebugPrint, msg, ...) _DebugPrint(LOG_ERROR, __FILENAME__, __LINE__, msg, ##__VA_ARGS__)
#define LFATAL(_DebugPrint, msg, ...) _DebugPrint(LOG_FATAL, __FILENAME__, __LINE__, msg, ##__VA_ARGS__)
