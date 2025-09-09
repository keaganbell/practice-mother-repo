#ifndef base_h
#define base_h

#define kiB (1<<10)
#define MiB (1<<20)
#define GiB (1<<30)
#define UINT16_MAX (0xffff)

/*
 * ///////////////////// Memory ////////////////////////////////////
 */
#define DEFAULT_ARENA_COMMIT_SIZE 4096
#define DEFAULT_ARENA_RESERVE_SIZE 2147483648
typedef struct allocator_arena {
    u64 reserved;
    u64 committed;
    u64 used;
    void *memory;
} arena_allocator;

typedef struct entire_file {
    u8 *contents;
    u64 size;
} entire_file;

#define array_count(array) (sizeof(array)/sizeof(array[0]))
#define _array_header_ struct { u64 count; u64 capacity; }
typedef struct array_header {
    u64 count;
    u64 capacity;
} array_header;

#endif // base_h
