#include "VulkanGraphicsPipelineFactory.h"
#include "VulkanGraphicsPipeline.h"
#include "RenderingWrapper.h"
#include "VkRenderingUnwrapper.h"
#include <CorePrecompiled.h>
#include "VkShaderFactory.h"

VulkanGraphicsPipelineFactory* VulkanGraphicsPipelineFactory::instance = nullptr;

VkPrimitiveTopology VulkanGraphicsPipelineFactory::UnwrapPrimitiveInfo(PrimtiveType * primitive)
{
    switch (*primitive)
    {
    case PrimtiveType::TOPOLOGY_POINT_LIST:
        return VkPrimitiveTopology::VK_PRIMITIVE_TOPOLOGY_POINT_LIST;

    case PrimtiveType::TOPOLOGY_LINE_LIST:
        return VkPrimitiveTopology::VK_PRIMITIVE_TOPOLOGY_LINE_LIST;

    case PrimtiveType::TOPOLOGY_LINE_STRIP:
        return VkPrimitiveTopology::VK_PRIMITIVE_TOPOLOGY_LINE_STRIP;

    case PrimtiveType::TOPOLOGY_TRIANGLE_LIST:
        return VkPrimitiveTopology::VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;

    case PrimtiveType::TOPOLOGY_TRIANGLE_STRIP:
        return VkPrimitiveTopology::VK_PRIMITIVE_TOPOLOGY_TRIANGLE_STRIP;

    case PrimtiveType::TOPOLOGY_TRIANGLE_FAN:
        return VkPrimitiveTopology::VK_PRIMITIVE_TOPOLOGY_TRIANGLE_FAN;

    case PrimtiveType::TOPOLOGY_LINE_LIST_WITH_ADJACENCY:
        return VkPrimitiveTopology::VK_PRIMITIVE_TOPOLOGY_LINE_LIST_WITH_ADJACENCY;

    case PrimtiveType::TOPOLOGY_LINE_STRIP_WITH_ADJACENCY:
        return VkPrimitiveTopology::VK_PRIMITIVE_TOPOLOGY_LINE_STRIP_WITH_ADJACENCY;

    case PrimtiveType::TOPOLOGY_TRIANGLE_LIST_WITH_ADJACENCY:
        return VkPrimitiveTopology::VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST_WITH_ADJACENCY;

    case PrimtiveType::TOPOLOGY_TRIANGLE_STRIP_WITH_ADJACENCY:
        return VkPrimitiveTopology::VK_PRIMITIVE_TOPOLOGY_TRIANGLE_STRIP_WITH_ADJACENCY;

    case PrimtiveType::TOPOLOGY_PATCH_LIST:
        return VkPrimitiveTopology::VK_PRIMITIVE_TOPOLOGY_PATCH_LIST;

    case PrimtiveType::TOPOLOGY_MAX_ENUM:
        return VkPrimitiveTopology::VK_PRIMITIVE_TOPOLOGY_MAX_ENUM;
    }

    ASSERT_MSG(0, "invalid primitive");
    return VkPrimitiveTopology::VK_PRIMITIVE_TOPOLOGY_MAX_ENUM;
}
//deprecated
VkVertexInputAttributeDescription VulkanGraphicsPipelineFactory::UnwrapVertexInputAttributeInfo(VertexInputAttributeInfo * info)
{
    VkVertexInputAttributeDescription obj = {};
    obj.binding = info->binding;
    obj.location = info->location;
    obj.offset = info->offset;
    obj.format = UnWrapFormat(info->format);

    return obj;
}

VkVertexInputAttributeDescription * VulkanGraphicsPipelineFactory::UnwrapVertexInputAttributeInfo(VertexInputAttributeInfo * info, const uint32_t & count)
{
    VkVertexInputAttributeDescription * inputAttributeDesc = new VkVertexInputAttributeDescription[count];
    for (uint32_t i = 0; i < count; i++)
    {
        inputAttributeDesc[i].binding = info->binding;
        inputAttributeDesc[i].location = info->location;
        inputAttributeDesc[i].offset = info->offset;
        inputAttributeDesc[i].format = UnWrapFormat(info->format);
    }
    return inputAttributeDesc;
}

//deprecated
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

VkVertexInputBindingDescription * VulkanGraphicsPipelineFactory::UnwrapVertexInputBindingInfo(VertexInputBindingInfo * info, const uint32_t & count)
{
    VkVertexInputBindingDescription * inputBindingDesc = new VkVertexInputBindingDescription[count];
    for (uint32_t i = 0; i < count; i++)
    {
        inputBindingDesc[i].binding = info->binding;
        inputBindingDesc[i].stride = info->stride;

        if (info->inputRate == VertexIputRate::PerVertex)
            inputBindingDesc[i].inputRate = VkVertexInputRate::VK_VERTEX_INPUT_RATE_VERTEX;
        else if (info->inputRate == VertexIputRate::PerInstance)
            inputBindingDesc[i].inputRate = VkVertexInputRate::VK_VERTEX_INPUT_RATE_VERTEX;
        else
            ASSERT_MSG(0, "Vertex input rate incorrect");
    }

    return inputBindingDesc;
}

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
    //vertexinput
    {
        std::map<uint32_t, VkPipelineVertexInputStateCreateInfo *>::iterator it;
        for (it = idToVertexInputMap.begin(); it != idToVertexInputMap.end(); it++)
        {
            delete it->second;
        }
        idToVertexInputMap.clear();
    }
    //input assembly
    {
        std::map<uint32_t, VkPipelineInputAssemblyStateCreateInfo*>::iterator it;
        for (it = idToInputAssemblyMap.begin(); it != idToInputAssemblyMap.end(); it++)
        {
            delete it->second;
        }
        idToInputAssemblyMap.clear();
    }
    //shader stage
    {
        std::vector<VkShaderStageWrapper *>::iterator it;
        for (it = shaderStageWrapperList.begin(); it != shaderStageWrapperList.end(); it++)
        {
            delete[](*it)->shaderStageCreateInfo;
            delete (*it);
        }
        shaderStageWrapperList.clear();
    }

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

void VulkanGraphicsPipelineFactory::CreateVertexInputState(const VertexInputWrapper * vertexInputWrapper)
{
    VertexInputState * inputState = vertexInputWrapper->inputState;

    VkPipelineVertexInputStateCreateInfo * pipelineVertexInputStateCreateInfo = new VkPipelineVertexInputStateCreateInfo;
    pipelineVertexInputStateCreateInfo->sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
    pipelineVertexInputStateCreateInfo->pVertexAttributeDescriptions = UnwrapVertexInputAttributeInfo ( inputState->attribInfo, vertexInputWrapper->inputState->attribCount);
    pipelineVertexInputStateCreateInfo->pVertexBindingDescriptions = UnwrapVertexInputBindingInfo(inputState->bindingInfo, inputState->bindingCount);
    pipelineVertexInputStateCreateInfo->vertexAttributeDescriptionCount = inputState->attribCount;
    pipelineVertexInputStateCreateInfo->vertexBindingDescriptionCount = inputState->bindingCount;
    pipelineVertexInputStateCreateInfo->pNext = nullptr;
    pipelineVertexInputStateCreateInfo->flags = 0;

    idToVertexInputMap.insert( std::pair<uint32_t, VkPipelineVertexInputStateCreateInfo *>(
    {const_cast<VertexInputWrapper *> (vertexInputWrapper)->GetId(), 
        pipelineVertexInputStateCreateInfo }) );
}

void VulkanGraphicsPipelineFactory::CreateInputAssemblyState(const InputAssemblyWrapper * inputAssemblyWrapper)
{
    VkPipelineInputAssemblyStateCreateInfo * pipelineInputAssemblyStateCreateInfo = new VkPipelineInputAssemblyStateCreateInfo;
    InputAssemblyState * inputAssembly = inputAssemblyWrapper->assemblyState;
    pipelineInputAssemblyStateCreateInfo->primitiveRestartEnable = inputAssembly->isPrimtiveRestartEnabled;
    pipelineInputAssemblyStateCreateInfo->sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
    pipelineInputAssemblyStateCreateInfo->topology = UnwrapPrimitiveInfo(inputAssembly->primitiveType);
    pipelineInputAssemblyStateCreateInfo->flags = 0;
    pipelineInputAssemblyStateCreateInfo->pNext = nullptr;

    idToInputAssemblyMap.insert(std::pair<uint32_t, VkPipelineInputAssemblyStateCreateInfo *>(
    { const_cast<InputAssemblyWrapper *> (inputAssemblyWrapper)->GetId(),
            pipelineInputAssemblyStateCreateInfo })
    );
}

void VulkanGraphicsPipelineFactory::CreateShaderState(const ShaderStateWrapper * shaderStateWrapper)
{
    VkShaderStageWrapper * vkStateWrapper = new VkShaderStageWrapper;
    vkStateWrapper->shaderCount = shaderStateWrapper->shaderCount;

    vkStateWrapper->shaderStageCreateInfo = new VkPipelineShaderStageCreateInfo[vkStateWrapper->shaderCount];

    for (uint32_t i = 0; i < vkStateWrapper->shaderCount; i++)
    {
        VkPipelineShaderStageCreateInfo * vkInfo = vkStateWrapper->shaderStageCreateInfo;
        uint32_t shaderId = shaderStateWrapper->shader[i].shaderId;
        vkInfo->sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
        vkInfo->pName = "main";
        vkInfo->pNext = nullptr;
        vkInfo->pSpecializationInfo = nullptr;
        vkInfo->flags = 0;
        vkInfo->module = *VkShaderFactory::GetInstance()->GetShaderModule(shaderId) ;
        vkInfo->stage = VkShaderFactory::GetInstance()->GetShaderStageFlag(shaderId);
    }

    shaderStageWrapperList.push_back(vkStateWrapper);
}

void VulkanGraphicsPipelineFactory::InitPipelineCache()
{

}

void VulkanGraphicsPipelineFactory::CreatePipelineShaderStage()
{

}

void VulkanGraphicsPipelineFactory::CreateDynamicState()
{
    pipelineDynamicStateCreateInfoObj.dynamicStateCount = (uint32_t)dynamicStateList.size();
    pipelineDynamicStateCreateInfoObj.pDynamicStates = dynamicStateList.data();
    pipelineDynamicStateCreateInfoObj.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
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
