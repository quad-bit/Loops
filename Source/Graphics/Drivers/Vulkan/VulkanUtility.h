#pragma once

#include <vulkan\vulkan.h>

void ErrorCheck(VkResult result);

struct CoreObjects
{
    static VkInstance * instanceObj;
    static VkPhysicalDevice * physicalDeviceObj;
    static VkDevice* logicalDeviceObj;
    static VkAllocationCallbacks * pAllocator;
};
