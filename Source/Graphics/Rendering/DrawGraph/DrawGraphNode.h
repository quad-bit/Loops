#pragma once
#include "Settings.h"
#include "RenderingWrapper.h"
#include <CorePrecompiled.h>
#include <vector>

#if (RENDERING_API == VULKAN)
#include "VulkanInterface.h"
typedef VulkanInterface ApiInterface;
#elif (RENDERING_API == DX)
#include "DxInterface.h"
typedef DxInterface ApiInterface
#endif

template <typename T>
class DrawCommandBuffer;

enum class DrawNodeTypes
{
    PIPELINE, // cmdBindPipeline
    CAMERA,
    LIGHT,
    LIGHT_GROUP,
    TEXTURE,
    SAMPLER,
    MATERIAL_SURFACE,
    MESH,
    TRANSFORM,
    DRAWING
};

namespace DrawGraphUtil
{
    extern uint32_t pipelineLayoutId;
    extern std::vector<uint32_t> descriptorIdList;
    extern uint32_t setOffset;
}

class DrawGraphNode
{
protected:

public:
    DrawNodeTypes drawNodeType;
    uint32_t setLevel;
    std::vector<SetWrapper*> setWrapperList;// pipeline node can have multiple sets wrappers
    std::vector<uint32_t> meshList;
    uint32_t numMeshes;
    virtual void Entry() = 0;
    virtual void Exit();
    
    static DrawCommandBuffer<ApiInterface> * dcb;
};

