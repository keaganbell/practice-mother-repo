#pragma once

typedef signed char      i8;
typedef signed short     i16;
typedef signed int       i32;
typedef signed long long i64;

typedef unsigned char      u8;
typedef unsigned short     u16;
typedef unsigned int       u32;
typedef unsigned long long u64;

typedef unsigned char  c8;
typedef unsigned short c16;

typedef float  f32;
typedef double f64;

typedef signed char      b8;
typedef signed short     b16;
typedef signed int       b32;
typedef signed long long b64;

#define  true 1
#define false 0
#define kab_assert(x) if (!(x)) *((u8 *)0) = 1

#define kiB (1024)
#define MiB (1024*1024)
#define GiB (1024*1024*1024ull)

#define UINT16_MAX (0xffff)
#define UINT32_MAX (0xffffffff)
#define array_count(v) (sizeof(v)/sizeof(v[0]))

typedef struct {
    c8 *value;
    u64 length;
} string_c8;

typedef struct {
    c16 *value;
    u64 length;
} string_c16;

typedef struct {
    string_c8 string;
    u64 capacity;
} string_builder_c8;

typedef struct {
    string_c16 string;
    u64 capacity;
} string_builder_c16;

#define string_lit_c8(x)   (string_c8){ .value = (x), .length = sizeof((x)) - 1 }
#define string_lit_c16(x) (string_c16){ .value = (x), .length = sizeof((x))/2 - 1 }

#if defined(_WIN64)
#define platform_alloc(size)               win32_virtual_alloc(NULL, size, MEM_RESERVE | MEM_COMMIT)
#define platform_reserve_memory(size)      win32_virtual_alloc(NULL, size, MEM_RESERVE)
#define platform_commit_memory(ptr, size)  win32_virtual_alloc(ptr,  size, MEM_COMMIT)
#define platform_free(ptr)                 win32_virtual_free(ptr)
#else
#error Platform not supported
#endif
