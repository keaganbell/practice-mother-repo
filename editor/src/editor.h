#pragma once
#include "defines.h"
#include "maths.h"
#include "input.h"
#include "arena.h"

struct program_memory {
    void *PermanentStorage;
    size_t PermanentStorageSize;

    void *TransientStorage;
    size_t TransientStorageSize;

    void (*DebugPrint)(char *Message, ...);
};

struct editor_state {
    memory_arena PermanentArena;
    memory_arena TransientArena;
};

#define INITIALIZE(name) void name(program_memory *Memory)
typedef INITIALIZE(program_initialize);
INITIALIZE(InitializeStub) {
}

#define UPDATE_AND_RENDER(name) void name(program_memory *Memory, program_input *Input)
typedef UPDATE_AND_RENDER(program_update_and_render);
UPDATE_AND_RENDER(UpdateAndRenderStub) {
}
