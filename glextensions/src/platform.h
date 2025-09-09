#pragma once

enum log_level;

#define PLATFORM_DEBUG_PRINT(name) void name(log_level Level, const char *Filename, u32 Line, const char *Message, ...)
typedef PLATFORM_DEBUG_PRINT(platform_debug_print);

struct platform_api {
    platform_debug_print *DebugPrint;
};

static platform_api *GlobalPlatform;
