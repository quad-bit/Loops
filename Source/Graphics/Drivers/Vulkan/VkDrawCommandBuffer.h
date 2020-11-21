#pragma once

#include <vulkan\vulkan.h>
#include <RenderingWrapper.h>

class VkDrawCommandBuffer
{
public:
    uint32_t id;
    VkCommandBuffer * commandBuffer;
    VkCommandPool * pool;

    PipelineType commandBufferType;
};