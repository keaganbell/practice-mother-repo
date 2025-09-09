#pragma once

static vulkan *WindowsInitVulkan() {
    LINFO("Setting up Vulkan on Windows...");
    vulkan *Vulkan = (vulkan *)PlatformAllocate(sizeof(vulkan));

    // TODO: query the platform for extensions here
    char *Extensions = "";
    b32 Success = InitVulkan(Vulkan, 0, &Extensions);
    if (!Success) {
        return NULL;
    }
    return Vulkan;
}
