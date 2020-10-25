#pragma once

#include <glm\glm.hpp>

using namespace glm;

namespace Loops::Core::Math
{
    class BoundingBox
    {
    private:
    	vec3 min, max, center;

    public:
    	BoundingBox();
    	BoundingBox(const BoundingBox& AABB);
    	virtual ~BoundingBox();

    	void CalculateBB(vec3 * vertices, int numVertices);
    };
}
