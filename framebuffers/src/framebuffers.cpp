#include "framebuffers.h"

#include "renderer.cpp"

static platform_api *Platform;

INITIALIZE(Initialize) {
    Platform = Memory->PlatformAPI;
    LINFO("Initialized.");
}

UPDATE_AND_RENDER(UpdateAndRender) {
    if (!Platform) {
        Platform = Memory->PlatformAPI;
    }
}
