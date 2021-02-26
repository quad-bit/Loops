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
//
//enum class ShaderResourceSetClassification
//{
//    Camera = 0,
//    Light = 1,
//    PerMat = 2,
//    ObjectSurface = 3
//
//};

//overall config for the entire uniform list for a binding
struct GlobalResourceSharingConfig
{
    uint32_t maxUniformPerResource; // per buffer or memory
    uint32_t allocatedUniformCount;
    //std::vector<uint32_t> uniformIdList;
};

struct GlobalResourceAllocationConfig
{
    uint32_t numDescriptors;
    uint32_t numResources;  // buffers/textures/samplers
    uint32_t numMemories;
};

//represent one binding, might have unique descriptor for each frame in flight
struct ShaderBindingDescription
{
    DescriptorType resourceType;
    uint32_t resourceId; // buffer/texture/sampler id
    uint32_t resourceMemoryId;
    uint32_t set;
    uint32_t binding;
    std::string resourceName;
    uint32_t numElements;
    size_t dataSizePerDescriptorAligned;
    size_t dataSizePerDescriptor; //sizeOf(Uniform Struct)
    uint32_t uniformId;
    uint32_t resParentId;
    COMPONENT_TYPE parentType;
    std::vector<size_t> offsetsForEachDescriptor;
    std::vector<uint32_t> descriptorIds;
    GlobalResourceSharingConfig sharingConfig;
    GlobalResourceAllocationConfig allocationConfig;
};

