#include "VkDrawCommandBuffer.h"
#include "VkRenderingUnwrapper.h"

void VkDrawCommandBuffer::SetViewport(const float & width, const float & height, const float & positionX, const float & positionY)
{
    VkViewport viewport;
    viewport.height = height;
    viewport.width = width;
    viewport.minDepth = 0.0f;
    viewport.maxDepth = 1.0f;
    viewport.x = positionX;
    viewport.y = positionY;
    vkCmdSetViewport(*commandBuffer, 0, 1, &viewport);
}

void VkDrawCommandBuffer::SetScissor(const float & width, const float & height, const float & positionX, const float & positionY)
{
    VkRect2D scissor;
    scissor.extent.width = (uint32_t)width;
    scissor.extent.height = (uint32_t)height;
    scissor.offset.x = (int32_t)positionX;
    scissor.offset.y = (int32_t)positionY;
    vkCmdSetScissor(*commandBuffer, 0, 1, &scissor);
}

void VkDrawCommandBuffer::BeginRenderPass(RenderPassBeginInfo * renderPassBeginInfo, SubpassContentStatus * subpassContentStatus)
{
    VkRenderPassBeginInfo vkRenderPassBeginInfo = UnwrapRenderPassBeginInfo(*renderPassBeginInfo);
    
    VkSubpassContents subpassContent;
    switch (*subpassContentStatus)
    {
    case SubpassContentStatus::SUBPASS_CONTENTS_INLINE:
        subpassContent = VkSubpassContents::VK_SUBPASS_CONTENTS_INLINE;
        break;

    case SubpassContentStatus::SUBPASS_CONTENTS_SECONDARY_COMMAND_BUFFERS:
        subpassContent = VkSubpassContents::VK_SUBPASS_CONTENTS_SECONDARY_COMMAND_BUFFERS;
        break;
    }

    vkCmdBeginRenderPass(*commandBuffer, &vkRenderPassBeginInfo, subpassContent);

    delete vkRenderPassBeginInfo.pClearValues;
}

void VkDrawCommandBuffer::EndRenderPass()
{
    //vkEndCommandBuffer(*commandBuffer);
    vkCmdEndRenderPass(*commandBuffer);
}
