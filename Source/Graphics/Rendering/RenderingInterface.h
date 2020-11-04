#pragma once
#include <Settings.h>

namespace Loops::Graphics::Rendering
{
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
        ForwardRendering<Loops::Graphics::Rendering::VulkanInterface> * apiInterface;
#elif (RENDERING_API == DX)
        ForwardRendering<DxInterface> * apiInterface;
#endif

    public:
        void Init();
        void SetupRenderer();
        void DislogeRenderer();
        void DeInit();
    };
}