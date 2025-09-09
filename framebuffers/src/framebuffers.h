#pragma once
#include "defines.h"
#include "platform.h"
#include "log.h"
#include "maths.h"
#include "renderer.h"

struct program_memory {
    platform_api *PlatformAPI;
};

#define INITIALIZE(name) void name(program_memory *Memory)
typedef INITIALIZE(program_initialize);
INITIALIZE(InitializeStub) {
}

#define UPDATE_AND_RENDER(name) void name(program_memory *Memory, render_commands *Commands)
typedef UPDATE_AND_RENDER(program_update_and_render);
UPDATE_AND_RENDER(UpdateAndRenderStub) {
}

struct program {
};
