static void win32_debug_print(const char *fmt, ...) {
    char buffer[1024];
    va_list args;
    va_start(args, fmt);
    vsnprintf(buffer, sizeof(buffer), fmt, args);
    va_end(args);
    OutputDebugString(buffer);
}

static void *win32_virtual_alloc(void *ptr, u64 size, u32 flags) {
    void *result = NULL;
    if (flags & MEM_RESERVE) {
        result = VirtualAlloc(ptr, size, flags, PAGE_READWRITE);
    }
    else {
        if (flags & MEM_COMMIT) {
            if (ptr) {
                result = VirtualAlloc(ptr, size, flags, PAGE_READWRITE);
            }
            else {
                win32_debug_print("ERROR: Call to commit memory without reserving it first.\n");
            }
        }
    }
    return result;
}

static void win32_virtual_free(void **ptr) {
    if (ptr && *ptr) {
        VirtualFree(*ptr, 0, MEM_RELEASE);
        *ptr = NULL;
    }
    else {
        win32_debug_print("ERROR: Call to free NULL pointer.\n");
        kab_assert(0);
    }
}

static inline u64 win32_get_frequency() {
    LARGE_INTEGER frequency;
	QueryPerformanceFrequency(&frequency);
    return frequency.QuadPart;
}

static inline u64 win32_get_counter() {
    LARGE_INTEGER counter;
	QueryPerformanceCounter(&counter);
    return counter.QuadPart;
}

static inline u64 win32_read_cpu_timer() {
    return __rdtsc();
}

static inline u64 win32_estimate_cpu_frequency() {
    u64 ms_to_wait = 100;
    u64 os_freq = win32_get_frequency();
    u64 os_wait_count = os_freq*ms_to_wait/1000;
    u64 os_counter_end = 0;
    u64 os_counter_elapsed = 0;
    u64 os_counter_start = win32_get_counter();
    u64 cpu_timer_start = win32_read_cpu_timer();
    while (os_counter_elapsed < os_wait_count) {
        os_counter_end = win32_get_counter();
        os_counter_elapsed = os_counter_end - os_counter_start;
    }
    u64 cpu_timer_end = win32_read_cpu_timer();
    u64 cpu_timer_elapsed = cpu_timer_end - cpu_timer_start;
    u64 cpu_frequency = 0;
    if (os_counter_elapsed) {
        cpu_frequency = cpu_timer_elapsed*os_freq/os_counter_elapsed;
    }
    return cpu_frequency;
}


