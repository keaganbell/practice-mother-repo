#ifndef windows_platform_h
#define windows_platform_h

#define platform_alloc(size)               win32_virtual_alloc(NULL, size, MEM_RESERVE | MEM_COMMIT)
#define platform_reserve_memory(size)      win32_virtual_alloc(NULL, size, MEM_RESERVE)
#define platform_commit_memory(ptr, size)  win32_virtual_alloc(ptr,  size, MEM_COMMIT)
#define platform_free(pointer)             win32_virtual_free(pointer)

#endif // windows_platform_h
