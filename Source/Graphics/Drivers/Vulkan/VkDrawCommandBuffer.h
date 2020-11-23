#pragma once

#include <vulkan\vulkan.h>
#include <RenderingWrapper.h>

class VkDrawCommandBuffer
{
public:
    uint32_t id;
    VkCommandBuffer * commandBuffer;
    VkCommandPool * pool;
    VkCommandBufferLevel level;
    PipelineType commandBufferType;

    void SetViewport(const float & width, const float & height, const float & positionX, const float & positionY);
    void SetScissor(const float & width, const float & height, const float & positionX, const float & positionY);
    void BeginRenderPass(RenderPassBeginInfo * renderPassBeginInfo, SubpassContentStatus * subpassContentStatus);
    void EndRenderPass();
};