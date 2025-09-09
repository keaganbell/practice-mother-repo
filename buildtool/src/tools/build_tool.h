#ifndef build_tool_h
#define build_tool_h

typedef struct command {
    arena_allocator arena;
    string_c8 source;
    string_builder_c8 cflags;
    string_builder_c8 linker;
} command;

#endif // build_tool_h
