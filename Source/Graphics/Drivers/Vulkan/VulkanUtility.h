#pragma once

#include <vulkan\vulkan.h>

namespace Loops::Graphics::Vulkan
{
    void ErrorCheck(VkResult result);

    struct CoreObjects
    {
        static VkInstance * instanceObj;
        static VkPhysicalDevice * physicalDeviceObj;
        static VkDevice* logicalDeviceObj;
        static VkAllocationCallbacks * pAllocator;
    };
}