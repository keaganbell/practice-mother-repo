#include "defines.h"
#include "log.h"
#include "platform.h"
#include "hotreload.h"

static platform_api *GlobalPlatform;

extern "C"{

INITIALIZE(Initialize) {
    GlobalPlatform = Platform;
    LINFO(Platform->DebugPrint, "Program is initialized.");
}

UPDATE_AND_RENDER(UpdateAndRender) {
    GlobalPlatform = Platform;
    LINFO(Platform->DebugPrint, "Test");
}

}
