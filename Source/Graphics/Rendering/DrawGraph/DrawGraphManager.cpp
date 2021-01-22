#include "DrawGraphManager.h"
#include "Settings.h"

#if (RENDERING_API == VULKAN)

#include "VulkanInterface.h"

DrawGraphManager<VulkanInterface>* DrawGraphManager<VulkanInterface>::instance = nullptr;

DrawGraphManager<VulkanInterface> * DrawGraphManager<VulkanInterface>::GetInstance()
{
    if (instance == nullptr)
    {
        instance = new DrawGraphManager();
    }
    return instance;
}
#elif (RENDERING_API == DX)

#include "DxInterface.h"

DrawGraphManager<DxInterface>* DrawGraphManager<DxInterface>::instance = nullptr;
DrawGraphManager<DxInterface> * DrawGraphManager<DxInterface>::GetInstance()
{
    if (instance == nullptr)
    {
        instance = new DrawGraphManager();
    }
    return instance;
}

#endif

