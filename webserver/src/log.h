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

#define Assert(val) { if (!(val)) { *(u8 *)0 = 1; } }

#define LTRACE(msg, ...) _DebugPrint(LOG_TRACE, __FILENAME__, __LINE__, msg, ##__VA_ARGS__)
#define LDEBUG(msg, ...) _DebugPrint(LOG_DEBUG, __FILENAME__, __LINE__, msg, ##__VA_ARGS__)
#define  LINFO(msg, ...) _DebugPrint(LOG_INFO,  __FILENAME__, __LINE__, msg, ##__VA_ARGS__)
#define  LWARN(msg, ...) _DebugPrint(LOG_WARN,  __FILENAME__, __LINE__, msg, ##__VA_ARGS__)
#define LERROR(msg, ...) _DebugPrint(LOG_ERROR, __FILENAME__, __LINE__, msg, ##__VA_ARGS__)
#define LFATAL(msg, ...) _DebugPrint(LOG_FATAL, __FILENAME__, __LINE__, msg, ##__VA_ARGS__)

static void _DebugPrint(log_level Level, const char *Filename, u32 Line, const char *Message, ...) {
    char Buffer[1024];
    i32 PrefixLength = 0;

    switch(Level) {
        case LOG_TRACE: {
            PrefixLength = snprintf(Buffer, sizeof(Buffer), "trace (%s:%u): ", Filename, Line);
        } break;

        case LOG_DEBUG: {
            PrefixLength = snprintf(Buffer, sizeof(Buffer), "debug (%s:%u): ", Filename, Line);
        } break;

        case LOG_INFO: {
            PrefixLength = snprintf(Buffer, sizeof(Buffer), "info  (%s:%u): ", Filename, Line);
        } break;

        case LOG_WARN: {
            PrefixLength = snprintf(Buffer, sizeof(Buffer), "warn  (%s:%u): ", Filename, Line);
        } break;

        case LOG_ERROR: {
            PrefixLength = snprintf(Buffer, sizeof(Buffer), "error (%s:%u): ", Filename, Line);
        } break;

        case LOG_FATAL: {
            PrefixLength = snprintf(Buffer, sizeof(Buffer), "FATAL (%s:%u): ", Filename, Line);
        } break;
    }
    
    va_list Args;
    va_start(Args, Message);
    vsnprintf(Buffer + PrefixLength, sizeof(Buffer) - PrefixLength, Message, Args);
    va_end(Args);
    snprintf(Buffer + strlen(Buffer), sizeof(Buffer) - strlen(Buffer), "\n");

    printf(Buffer);
}
