#pragma once

#define PLATFORM_DEBUG_PRINT(name) void name(const char *Message, ...)
typedef PLATFORM_DEBUG_PRINT(platform_debug_print);

#define PLATFORM_VIRTUAL_ALLOCATE(name) void *name(size_t Size)
typedef PLATFORM_VIRTUAL_ALLOCATE(platform_virtual_allocate);

struct platform_api {
    platform_debug_print *DebugPrint;
    platform_virtual_allocate *VirtualAllocate;
};
