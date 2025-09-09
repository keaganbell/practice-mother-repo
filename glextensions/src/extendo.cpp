#include "defines.h"
#include "platform.h"
#include "log.h"
#include "arena.h"
#include "extendo.h"

extern "C"{

INITIALIZE(Initialize) {
    GlobalPlatform = Platform;
    program_state *State = (program_state *)Memory->PermanentStorage;
    State->PermanentArena = InitializeArena((u8 *)Memory->PermanentStorage, Memory->PermanentSize);
    State->PermanentArena.Used = sizeof(*State);

    State->ScratchArena = InitializeArena((u8 *)Memory->FrameScratchStorage, Memory->FrameScratchSize);
}

UPDATE_AND_RENDER(UpdateAndRender) {
    GlobalPlatform = Platform;
    program_state *State = (program_state *)Memory->PermanentStorage;
    State->ScratchArena = InitializeArena((u8 *)Memory->FrameScratchStorage, Memory->FrameScratchSize);
    memory_arena *Scratch = &State->ScratchArena;
}

}
