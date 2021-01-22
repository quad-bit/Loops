#pragma once
#include "Settings.h"

#include <CorePrecompiled.h>

#if (RENDERING_API == VULKAN)
#include "VulkanInterface.h"
typedef VulkanInterface ApiInterface;
#elif (RENDERING_API == DX)
#include "DxInterface.h"
typedef DxInterface ApiInterface
#endif

template <typename T>
class DrawCommandBuffer;

class DrawGraphNode
{
protected:

public:
    virtual void Execute() = 0;
    static DrawCommandBuffer<ApiInterface> * dcb;
};

