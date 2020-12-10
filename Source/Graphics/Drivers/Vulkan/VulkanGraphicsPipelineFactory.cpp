#include "VulkanGraphicsPipelineFactory.h"
#include "VulkanGraphicsPipeline.h"
#include "RenderingWrapper.h"
#include "VkRenderingUnwrapper.h"
#include <CorePrecompiled.h>

VulkanGraphicsPipelineFactory* VulkanGraphicsPipelineFactory::instance = nullptr;

void VulkanGraphicsPipelineFactory::Init()
{
    PLOGD << "VulkanGraphicsPipelineFactory init";

    dynamicStateList.push_back(VK_DYNAMIC_STATE_VIEWPORT);
    dynamicStateList.push_back(VK_DYNAMIC_STATE_SCISSOR);
    
    //VulkanGraphicsPipeline::mandatoryStates.push_back(PipelineStates::VertexInputState);
}

void VulkanGraphicsPipelineFactory::DeInit()
{
    PLOGD << "VulkanGraphicsPipelineFactory Deinit";

    size_t numObjects = pipelineList.size();
    for (uint32_t i = 0; i < numObjects; i++)
    {
        delete pipelineList[i];
    }
}

void VulkanGraphicsPipelineFactory::Update()
{

}

VulkanGraphicsPipelineFactory * VulkanGraphicsPipelineFactory::GetInstance()
{
    if (instance == nullptr)
    {
        instance = new VulkanGraphicsPipelineFactory();
    }
    return instance;
}

VulkanGraphicsPipelineFactory::~VulkanGraphicsPipelineFactory()
{

}

void VulkanGraphicsPipelineFactory::InitiatePipelineCreation(uint32_t id, VertexInputAttributeInfo * inputAttributeInfo, const uint32_t &  inputAttribCount,
    VertexInputBindingInfo * inputBindingInfo, const uint32_t & inputBindingCount)
{
    VkVertexInputBindingDescription * inputBindingDesc = new VkVertexInputBindingDescription[inputAttribCount];
    for(uint32_t i = 0; i < inputBindingCount; i++)
        inputBindingDesc[i] = UnwrapVertexInputBindingInfo(&inputBindingInfo[i]);

    VkVertexInputAttributeDescription * inputAttributeDesc = new VkVertexInputAttributeDescription[inputAttribCount];
    for (uint32_t i = 0; i < inputAttribCount; i++)
        inputAttributeDesc[i] = UnwrapVertexInputAttributeInfo(&inputAttributeInfo[i]);

    VkPipelineVertexInputStateCreateInfo * pipelineVertexInputStateCreateInfo = new VkPipelineVertexInputStateCreateInfo;
    pipelineVertexInputStateCreateInfo->sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
    pipelineVertexInputStateCreateInfo->pVertexAttributeDescriptions = inputAttributeDesc;
    pipelineVertexInputStateCreateInfo->pVertexBindingDescriptions = inputBindingDesc;
    pipelineVertexInputStateCreateInfo->vertexAttributeDescriptionCount = inputAttribCount;
    pipelineVertexInputStateCreateInfo->vertexBindingDescriptionCount = inputBindingCount;
    pipelineVertexInputStateCreateInfo->pNext = nullptr;
    pipelineVertexInputStateCreateInfo->flags = 0;

    VulkanGraphicsPipeline * obj = new VulkanGraphicsPipeline(id);
    obj->SetPipelineVertexInputState(pipelineVertexInputStateCreateInfo);

    pipelineList.push_back(obj);
}

void VulkanGraphicsPipelineFactory::InitPipelineCache()
{

}

void VulkanGraphicsPipelineFactory::CreatePipelineShaderStage()
{

}

void VulkanGraphicsPipelineFactory::CreateDynamicState()
{
    pipelineDynamicStateCreateInfoObj.dynamicStateCount = dynamicStateList.size();
    pipelineDynamicStateCreateInfoObj.pDynamicStates = dynamicStateList.data();
    pipelineDynamicStateCreateInfoObj.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
}

VkVertexInputAttributeDescription VulkanGraphicsPipelineFactory::UnwrapVertexInputAttributeInfo(VertexInputAttributeInfo * info)
{
    VkVertexInputAttributeDescription obj = {};
    obj.binding = info->binding;
    obj.location = info->location;
    obj.offset = info->offset;
    obj.format = UnWrapFormat(info->format);

    return obj;
}

VkVertexInputBindingDescription VulkanGraphicsPipelineFactory::UnwrapVertexInputBindingInfo(VertexInputBindingInfo * info)
{
    VkVertexInputBindingDescription obj = {};
    obj.binding = info->binding;
    obj.stride = info->stride;

    if (info->inputRate == VertexIputRate::PerVertex)
        obj.inputRate = VkVertexInputRate::VK_VERTEX_INPUT_RATE_VERTEX;
    else if (info->inputRate == VertexIputRate::PerInstance)
        obj.inputRate = VkVertexInputRate::VK_VERTEX_INPUT_RATE_VERTEX;
    else
        ASSERT_MSG(0, "Vertex input rate incorrect");
    
    return obj;
}

void VulkanGraphicsPipelineFactory::CreatePipelineVertexInputState()
{

}

void VulkanGraphicsPipelineFactory::CreatePipelineInputAssemblyState()
{
    pipelineInputAssemblyStateCreateInfo.primitiveRestartEnable = VK_FALSE;
    pipelineInputAssemblyStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
    pipelineInputAssemblyStateCreateInfo.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
}

void VulkanGraphicsPipelineFactory::CreatePipelineTessellationState()
{
}

void VulkanGraphicsPipelineFactory::CreatePipelineRasterizationState()
{
    pipelineRasterizationStateCreateInfoObj.cullMode = VK_CULL_MODE_BACK_BIT;
    pipelineRasterizationStateCreateInfoObj.depthBiasClamp = 0;
    pipelineRasterizationStateCreateInfoObj.depthBiasConstantFactor = 0;
    pipelineRasterizationStateCreateInfoObj.depthBiasEnable = VK_FALSE;
    pipelineRasterizationStateCreateInfoObj.depthBiasSlopeFactor = 0;
    pipelineRasterizationStateCreateInfoObj.depthClampEnable = VK_FALSE;
    pipelineRasterizationStateCreateInfoObj.frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE;
    pipelineRasterizationStateCreateInfoObj.lineWidth = 1.0f;
    pipelineRasterizationStateCreateInfoObj.polygonMode = VK_POLYGON_MODE_FILL;
    pipelineRasterizationStateCreateInfoObj.rasterizerDiscardEnable = VK_FALSE;
    pipelineRasterizationStateCreateInfoObj.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
}

void VulkanGraphicsPipelineFactory::CreatePipelineColorBlendState()
{
    attachments[0].alphaBlendOp = VK_BLEND_OP_ADD;
    attachments[0].blendEnable = VK_FALSE;
    attachments[0].colorBlendOp = VK_BLEND_OP_ADD;
    attachments[0].colorWriteMask = 0xf;
    attachments[0].dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;
    attachments[0].dstColorBlendFactor = VK_BLEND_FACTOR_ZERO;
    attachments[0].srcAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;
    attachments[0].srcColorBlendFactor = VK_BLEND_FACTOR_ZERO;

    pipelineColorBlendStateCreateInfoObj.attachmentCount = 1;
    pipelineColorBlendStateCreateInfoObj.blendConstants[0] = 1.0f;
    pipelineColorBlendStateCreateInfoObj.blendConstants[1] = 1.0f;
    pipelineColorBlendStateCreateInfoObj.blendConstants[2] = 1.0f;
    pipelineColorBlendStateCreateInfoObj.blendConstants[3] = 1.0f;
    pipelineColorBlendStateCreateInfoObj.logicOp = VK_LOGIC_OP_NO_OP;
    pipelineColorBlendStateCreateInfoObj.logicOpEnable = VK_FALSE;
    pipelineColorBlendStateCreateInfoObj.pAttachments = attachments;
    pipelineColorBlendStateCreateInfoObj.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
}

void VulkanGraphicsPipelineFactory::CreatePipelineViewportState()
{
    pipelineViewportStateCreateInfoObj.pScissors = nullptr;
    pipelineViewportStateCreateInfoObj.pViewports = nullptr;
    pipelineViewportStateCreateInfoObj.scissorCount = 1;
    pipelineViewportStateCreateInfoObj.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
    pipelineViewportStateCreateInfoObj.viewportCount = 1;
}

void VulkanGraphicsPipelineFactory::CreatePipelineDepthStencilState()
{
    pipelineDepthStencilStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
    pipelineDepthStencilStateCreateInfo.pNext = NULL;
    pipelineDepthStencilStateCreateInfo.flags = 0;
    pipelineDepthStencilStateCreateInfo.depthTestEnable = VK_TRUE;
    pipelineDepthStencilStateCreateInfo.depthWriteEnable = VK_TRUE;
    pipelineDepthStencilStateCreateInfo.depthCompareOp = VK_COMPARE_OP_LESS_OR_EQUAL;
    pipelineDepthStencilStateCreateInfo.depthBoundsTestEnable = VK_FALSE;
    pipelineDepthStencilStateCreateInfo.minDepthBounds = 0;
    pipelineDepthStencilStateCreateInfo.maxDepthBounds = 0;
    pipelineDepthStencilStateCreateInfo.stencilTestEnable = VK_FALSE;
    pipelineDepthStencilStateCreateInfo.back.failOp = VK_STENCIL_OP_KEEP;
    pipelineDepthStencilStateCreateInfo.back.passOp = VK_STENCIL_OP_KEEP;
    pipelineDepthStencilStateCreateInfo.back.compareOp = VK_COMPARE_OP_ALWAYS;
    pipelineDepthStencilStateCreateInfo.back.compareMask = 0;
    pipelineDepthStencilStateCreateInfo.back.reference = 0;
    pipelineDepthStencilStateCreateInfo.back.depthFailOp = VK_STENCIL_OP_KEEP;
    pipelineDepthStencilStateCreateInfo.back.writeMask = 0;
    pipelineDepthStencilStateCreateInfo.front = pipelineDepthStencilStateCreateInfo.back;
}

void VulkanGraphicsPipelineFactory::CreatePipelineMultiSampleState()
{
    pipelineMultisampleStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
    pipelineMultisampleStateCreateInfo.pNext = NULL;
    pipelineMultisampleStateCreateInfo.flags = 0;
    pipelineMultisampleStateCreateInfo.pSampleMask = NULL;
    pipelineMultisampleStateCreateInfo.alphaToCoverageEnable = VK_FALSE;
    pipelineMultisampleStateCreateInfo.alphaToOneEnable = VK_FALSE;
    pipelineMultisampleStateCreateInfo.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
    pipelineMultisampleStateCreateInfo.sampleShadingEnable = VK_FALSE;
    pipelineMultisampleStateCreateInfo.minSampleShading = 0.0;
}
