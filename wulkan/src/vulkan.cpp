#pragma once

static b32 InitVulkan(vulkan *Vulkan, u32 ExtensionCount, char **EnabledExtensionNames) {
    LINFO("Initializing Vulkan...");

    VkApplicationInfo AppInfo = {};
    AppInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    AppInfo.pNext = NULL;
    AppInfo.pApplicationName = "Wulkan";
    AppInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
    AppInfo.pEngineName = "Wulkan Engine";
    AppInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
    AppInfo.apiVersion = VK_API_VERSION_1_0;

    char *EnabledLayerNames = "";

    VkInstanceCreateInfo InstanceInfo = {};
    InstanceInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    InstanceInfo.pNext = NULL;
    InstanceInfo.flags = 0;
    InstanceInfo.pApplicationInfo = &AppInfo;
    InstanceInfo.enabledLayerCount = ExtensionCount;
    InstanceInfo.ppEnabledLayerNames = EnabledExtensionNames;
    InstanceInfo.enabledExtensionCount = 0;
    InstanceInfo.ppEnabledExtensionNames = &EnabledLayerNames;

    if (vkCreateInstance(&InstanceInfo, NULL, &Vulkan->Instance) != VK_SUCCESS) {
        LERROR("Failed to create Vulkan instance");
        return false;
    }



    return true;
}
