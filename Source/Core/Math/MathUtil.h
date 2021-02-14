#pragma once

#define GLM_ENABLE_EXPERIMENTAL
#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>

namespace MathUtil
{
#define Vec4ToVec3(a) glm::vec3(a.x, a.y, a.z)
#define Vec3ToVec4_1(a) glm::vec4(a.x, a.y, a.z, 1.0f)
#define Vec3ToVec4_0(a) glm::vec4(a.x, a.y, a.z, 0.0f)

    float lerp(float x, float y, float t);
}