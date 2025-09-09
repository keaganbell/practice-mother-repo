#ifndef windows_platform_cpp
#define windows_platform_cpp

function void *PlatformAllocate(u64 size) {
    return VirtualAlloc(0, size, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
}

function void PlatformFree(void *memory) {
    VirtualFree(memory, 0, MEM_RELEASE);
}

function void PlatformOpenFile(String string) {
}

function void PlatformCloseFile(PlatformFile File) {
}

function EntireFile PlatformReadFile(Arena *arena, const char *filename) {
    EntireFile result = {};
    FILE *file = fopen(filename, "rb");
    if (file) {
        fseek(file, 0, SEEK_END);
        result.size = ftell(file);
        rewind(file);
        result.contents = PushArray(arena, char, result.size + 1);
        fread(result.contents, 1, result.size, file);
        result.contents[result.size] = '\0';
        fclose(file);
    }
    else {
        printf("Couldn't find file %s\n", filename);
    }
    return result;
}

#endif // windows_platform_cpp
