/*
 * ///////////////////    Arenas    ///////////////////////////////
 */
#define arena_push_struct(arena, type) (type *)_arena_push(arena, sizeof(type), true)
#define arena_push_array(arena, type, count) (type *)_arena_push(arena, sizeof(type)*(count), true)
#define arena_push_size(arena, size) (u8 *)_arena_push(arena, size, true)
void *_arena_push(arena_allocator *arena, u64 size, b32 clear_to_zero) {
    void *result = NULL;
    if (arena->memory == NULL) {
        arena->memory = platform_reserve_memory(DEFAULT_ARENA_RESERVE_SIZE);
        arena->reserved = DEFAULT_ARENA_RESERVE_SIZE;
        u64 committed = DEFAULT_ARENA_COMMIT_SIZE;
        while (size > committed) {
            committed += DEFAULT_ARENA_COMMIT_SIZE;
        }
        kbassert(arena->memory);
        platform_commit_memory(arena->memory, committed);
        arena->committed = committed;
    }
    if (arena->used + size >= arena->committed) {
        u64 new_committed = arena->committed;
        while (arena->committed + size > new_committed) {
            new_committed += DEFAULT_ARENA_COMMIT_SIZE;
        }
        kbassert(new_committed < arena->reserved);
        platform_commit_memory(arena->memory, new_committed);
        arena->committed = new_committed;
    }
    result = (u8 *)arena->memory + arena->used;
    arena->used += size;
    return result;
}

inline void arena_reset(arena_allocator *arena) {
    arena->used = 0;
}

inline void arena_destroy(arena_allocator *arena) {
    platform_free(arena->memory);
    arena->memory = NULL;
}

