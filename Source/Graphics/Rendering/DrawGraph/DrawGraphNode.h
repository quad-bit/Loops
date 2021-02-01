#pragma once
#include "Settings.h"
#include "RenderingWrapper.h"
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

enum class DrawNodeTypes
{
    PIPELINE,
    CAMERA,
    LIGHT,
    LIGHT_GROUP,
    TEXTURE,
    SAMPLER,
    MATERIAL_SURFACE,
    MESH,
    TRANSFORM
};


class GraphConnectionRequirement
{
public:
    std::vector<uint32_t> meshList;
    int numMeshes;// if -1 connect to all the incoming/outgoing nodes
};

class DrawGraphNode
{
protected:

public:
    DrawNodeTypes drawNodeType;
    uint32_t setLevel;
    SetWrapper * setWrapper;
    std::vector<uint32_t> meshList;
    uint32_t numMeshes;
    virtual void Execute() = 0;
    static DrawCommandBuffer<ApiInterface> * dcb;
};

