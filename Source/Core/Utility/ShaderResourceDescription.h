#pragma once

#include <glm\glm.hpp>
#include "RenderingWrapper.h"
#include "ECS_Helper.h"
#include <string>

enum class AllocationMethod
{
    IMMEDIATE,
    LAZY
};

struct ShaderResourceSettings
{
    uint32_t descriptorPerUniform;
    uint32_t resourcePerUniform;
    uint32_t memoryPerUniform;
};
//represent one binding, might have unique descriptor for each frame in flight
struct ShaderResourceDescription
{
    DescriptorType resourceType;
    uint32_t resourceId; // buffer/texture/sampler id
    uint32_t resourceMemoryId;
    uint32_t set;
    uint32_t binding;
    std::string resourceName;
    uint32_t numElements;
    size_t dataSizePerDescriptor;
    uint32_t uniformId;
    uint32_t resParentId;
    COMPONENT_TYPE parentType;
    std::vector<size_t> offsetsForEachDescriptor;
};

struct UniformResourceSharingConfig
{
    uint32_t maxUniformPerResource; // per buffer or memory
    uint32_t allocatedUniformCount;
    std::vector<uint32_t> uniformIdList;
};

struct UniformResourceAllocationConfig
{
    uint32_t numDescriptors;
    uint32_t numResources;  // buffers/textures/samplers
    uint32_t numMemories;
};

