#include "VulkanGraphicsPipeline.h"

void VulkanGraphicsPipeline::SetPipelineShaderStage(VkPipelineShaderStageCreateInfo * info) 
{
    pipelineShaderStageCreateInfo = info;
}

void VulkanGraphicsPipeline::SetPipelineVertexInputState(VkPipelineVertexInputStateCreateInfo * info)
{
    pipelineVertexInputStateCreateInfo = info;
}

void VulkanGraphicsPipeline::SetPipelineInputAssemblyState(VkPipelineInputAssemblyStateCreateInfo * info)
{
    pipelineInputAssemblyStateCreateInfo = info;
}

void VulkanGraphicsPipeline::SetPipelineTessellationState(VkPipelineTessellationStateCreateInfo * info)
{
    pipelineTessellationStateCreateInfo = info;
}

void VulkanGraphicsPipeline::SetPipelineViewportState(VkPipelineViewportStateCreateInfo * info)
{
    pipelineViewportStateCreateInfo = info;
}

void VulkanGraphicsPipeline::SetPipelineRasterizationState(VkPipelineRasterizationStateCreateInfo * info)
{ 
    pipelineRasterizationStateCreateInfo = info;
}

void VulkanGraphicsPipeline::SetPipelineMultiSampleState(VkPipelineMultisampleStateCreateInfo * info)
{
    pipelineMultisampleStateCreateInfo = info;
}

void VulkanGraphicsPipeline::SetPipelineDepthStencilState(VkPipelineDepthStencilStateCreateInfo * info)
{
    pipelineDepthStencilStateCreateInfo = info;
}

void VulkanGraphicsPipeline::SetPipelineColorBlendState(VkPipelineColorBlendStateCreateInfo * info)
{
    pipelineColorBlendStateCreateInfo = info;
}

void VulkanGraphicsPipeline::SetDynamicState(VkPipelineDynamicStateCreateInfo * info)
{
    pipelineDynamicStateCreateInfo = info;
}

void VulkanGraphicsPipeline::CreatePipeline()
{
}

VulkanGraphicsPipeline::VulkanGraphicsPipeline(const uint32_t & id)
{
    objectId = id;
}

VulkanGraphicsPipeline::~VulkanGraphicsPipeline()
{
    //delete[] pipelineVertexInputStateCreateInfo->pVertexAttributeDescriptions;
    //delete[] pipelineVertexInputStateCreateInfo->pVertexBindingDescriptions;
    //delete pipelineVertexInputStateCreateInfo;
}
