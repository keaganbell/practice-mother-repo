/*
 *  //////////////////////////  alloc   ////////////////////////////////
 */
inline void *win32_virtual_alloc(void *pointer, u64 size, u32 flags) {
    return VirtualAlloc(pointer, size, flags, PAGE_READWRITE);
}

inline void win32_virtual_free(void *memory) {
    VirtualFree(memory, 0, MEM_RELEASE);
}

/*
 *  //////////////////////////  files   //////////////////////////////////
 */
entire_file win32_read_entire_file(arena_allocator *arena, string_c16 filename) {
    LPCWSTR filepath = (LPCWSTR)filename.value;
    HANDLE file = CreateFileW(
        filepath,
        GENERIC_READ,
        FILE_SHARE_READ,
        NULL,
        OPEN_EXISTING,
        FILE_ATTRIBUTE_NORMAL,
        NULL
    );

    entire_file result = {};
    if (file != INVALID_HANDLE_VALUE) {
        LARGE_INTEGER size;
        if (GetFileSizeEx(file, &size)) {
            u64 file_size = (u64)size.QuadPart;
            result.contents = (char *)arena_push_size(arena, file_size + 1);
            if (result.contents) {
                DWORD bytes_read = 0;
                if (ReadFile(file, result.contents, file_size, &bytes_read, NULL)) {
                    if (bytes_read == file_size) {
                        result.size = file_size;
                    }
                }
                result.contents[file_size] = '\0';
            }
        }
        CloseHandle(file);
    }

    return result;
}

