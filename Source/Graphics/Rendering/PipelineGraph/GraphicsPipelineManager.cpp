#include "GraphicsPipelineManager.h"
#include "Settings.h"

#if (RENDERING_API == VULKAN)

#include "VulkanInterface.h"

GraphicsPipelineManager<VulkanInterface>* GraphicsPipelineManager<VulkanInterface>::instance = nullptr;

GraphicsPipelineManager<VulkanInterface> * GraphicsPipelineManager<VulkanInterface>::GetInstance()
{
    if (instance == nullptr)
    {
        instance = new GraphicsPipelineManager();
    }
    return instance;
}
#elif (RENDERING_API == DX)

#include "DxInterface.h"

GraphicsPipelineManager<DxInterface>* GraphicsPipelineManager<DxInterface>::instance = nullptr;
GraphicsPipelineManager<DxInterface> * GraphicsPipelineManager<DxInterface>::GetInstance()
{
    if (instance == nullptr)
    {
        instance = new GraphicsPipelineManager();
    }
    return instance;
}

#endif

void PipelineDrawNode::Execute()
{
}
