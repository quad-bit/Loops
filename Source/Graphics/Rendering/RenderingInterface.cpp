#include "RenderingInterface.h"
#include "ForwardInterface.h"
#include "VulkanInterface.h"


void RenderingInterface::Init()
{
#if (RENDERING_API == VULKAN)
    apiInterface = new ForwardRendering<VulkanInterface>();
#elif (RENDERING_API == DX)
    apiInterface = new ForwardRendering<DxInterface>();
#endif

    apiInterface->Init();
}

void RenderingInterface::SetupRenderer()
{
    apiInterface->SetupRenderer();
}

void RenderingInterface::DislogeRenderer()
{
    apiInterface->DislogeRenderer();
}

void RenderingInterface::DeInit()
{
    apiInterface->DeInit();
    delete apiInterface;
}
