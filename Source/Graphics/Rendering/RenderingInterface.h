#pragma once
#include <Settings.h>

template <typename T>
class ForwardRendering;

template <typename T>
class DeferredRendering;

class VulkanInterface;
class DxInterface;

class RenderingInterface
{
private:

#if (RENDERING_API == VULKAN)
        ForwardRendering<VulkanInterface> * apiInterface;
#elif (RENDERING_API == DX)
        ForwardRendering<DxInterface> * apiInterface;
#endif

public:
    void Init();
    void SetupRenderer();
    void DislogeRenderer();
    void DeInit();
};
