#pragma once

struct program_memory {
    void *PermanentStorage;
    size_t PermanentSize;

    void *FrameScratchStorage;
    size_t FrameScratchSize;
};

// #ifdef Win64
#include <windows.h>
#include <GL/wglext.h>

static void _PlatformDebugPrint(log_level Level, const char *Filename, u32 Line, const char *Message, ...) {
    char Buffer[1024];
    i32 PrefixLength = 0;

    switch(Level) {
        case LOG_TRACE: {
            PrefixLength = snprintf(Buffer, sizeof(Buffer), "trace: %s:%u: ", Filename, Line);
        } break;

        case LOG_DEBUG: {
            PrefixLength = snprintf(Buffer, sizeof(Buffer), "debug: %s:%u: ", Filename, Line);
        } break;

        case LOG_INFO: {
            PrefixLength = snprintf(Buffer, sizeof(Buffer), "info:  %s:%u: ", Filename, Line);
        } break;

        case LOG_WARN: {
            PrefixLength = snprintf(Buffer, sizeof(Buffer), "warn:  %s:%u: ", Filename, Line);
        } break;

        case LOG_ERROR: {
            PrefixLength = snprintf(Buffer, sizeof(Buffer), "error: %s:%u: ", Filename, Line);
        } break;

        case LOG_FATAL: {
            PrefixLength = snprintf(Buffer, sizeof(Buffer), "FATAL: %s:%u: ", Filename, Line);
        } break;
    }
    
    va_list Args;
    va_start(Args, Message);
    vsnprintf(Buffer + PrefixLength, sizeof(Buffer) - PrefixLength, Message, Args);
    va_end(Args);
    snprintf(Buffer + strlen(Buffer), sizeof(Buffer) - strlen(Buffer), "\n");

    OutputDebugString(Buffer);
}
