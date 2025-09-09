#ifndef windows_platform_h
#define windows_platform_h

struct PlatformFile {
    HANDLE Handle;
};

function void *PlatformAllocate(u64 size);
function void PlatformFree(void *memory);
function void PlatformOpenFile(String string);
function void PlatformCloseFile(PlatformFile File);
function EntireFile PlatformReadFile(Arena *arena, const char *filename);

#endif // windows_platform_h
