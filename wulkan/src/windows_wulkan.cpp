#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <stdio.h> // vsnprintf

#include "vulkan/vulkan.h"

#include "defines.h"
#include "log.h"
#include "vulkan.h"
#include "platform.h"

#include "vulkan.cpp"
#include "windows_vulkan.cpp"

static void *PlatformAllocate(size_t Size) {
    return VirtualAlloc(0, Size, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
}

static void PlatformDebugPrint(const char *Message, ...) {
    char Buffer[2048];
    va_list Args;
    va_start(Args, Message);
    vsnprintf(Buffer, sizeof(Buffer), Message, Args);
    va_end(Args);
    OutputDebugString(Buffer);
}

int WinMain(HINSTANCE Instance, HINSTANCE PrevInstance, PTSTR CommandLine, int CommandShow) {
    vulkan *Vulkan = WindowsInitVulkan();
    return 0;
}
