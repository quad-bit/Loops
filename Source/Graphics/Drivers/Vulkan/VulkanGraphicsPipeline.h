#pragma once
#include <vector>
#include <RenderingWrapper.h>
#include <vulkan\vulkan.h>

class VulkanGraphicsPipeline
{
private:
    uint32_t objectId;

    VkPipelineCache *                                     pipelineCacheObj;
    VkPipelineShaderStageCreateInfo *                     pipelineShaderStageCreateInfo;
    VkPipelineVertexInputStateCreateInfo *                pipelineVertexInputStateCreateInfo;
    VkPipelineInputAssemblyStateCreateInfo *              pipelineInputAssemblyStateCreateInfo;
    VkPipelineTessellationStateCreateInfo *               pipelineTessellationStateCreateInfo;
    VkPipelineViewportStateCreateInfo *                   pipelineViewportStateCreateInfo;
    VkPipelineRasterizationStateCreateInfo *              pipelineRasterizationStateCreateInfo;
    VkPipelineMultisampleStateCreateInfo *                pipelineMultisampleStateCreateInfo;
    VkPipelineDepthStencilStateCreateInfo *               pipelineDepthStencilStateCreateInfo;
    VkPipelineColorBlendStateCreateInfo *                 pipelineColorBlendStateCreateInfo;
    VkPipelineDynamicStateCreateInfo *                    pipelineDynamicStateCreateInfo;
    VkGraphicsPipelineCreateInfo *                        vulkanGraphicsPipelineCreateInfo;

public:
    void SetPipelineShaderStage(VkPipelineShaderStageCreateInfo * info);
    void SetPipelineVertexInputState(VkPipelineVertexInputStateCreateInfo * info);
    void SetPipelineInputAssemblyState(VkPipelineInputAssemblyStateCreateInfo * info);
    void SetPipelineTessellationState(VkPipelineTessellationStateCreateInfo * info);
    void SetPipelineViewportState(VkPipelineViewportStateCreateInfo * info);
    void SetPipelineRasterizationState(VkPipelineRasterizationStateCreateInfo * info);
    void SetPipelineMultiSampleState(VkPipelineMultisampleStateCreateInfo * info);
    void SetPipelineDepthStencilState(VkPipelineDepthStencilStateCreateInfo * info);
    void SetPipelineColorBlendState(VkPipelineColorBlendStateCreateInfo * info);
    void SetDynamicState(VkPipelineDynamicStateCreateInfo * info);
    void CreatePipeline();
  
    VulkanGraphicsPipeline(const uint32_t & id);
    ~VulkanGraphicsPipeline();
};
