#pragma once

#include <glm\glm.hpp>
#include "RenderingWrapper.h"

struct ShaderResource
{
    DescriptorType resourceType;
    uint32_t resourceId; // descriptorset id
    uint32_t set;
    uint32_t binding;
};


struct CameraUniform
{
    glm::vec3 camPos;
    glm::mat4 viewMat;
    glm::mat4 projectionMat;
};
