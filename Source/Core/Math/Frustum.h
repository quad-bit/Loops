#pragma once

#include "Vector.h"
#include "Plane.h"
#include <glm\glm.hpp>

using namespace Loops::Core::Utils::Container;

namespace Loops::Core::Math
{
    class Frustum
    {
    private:
    	Vector<Loops::Core::Math::Plane> frustum;

    public:
    	Frustum();
    	~Frustum();

    	void CalculateFrustum(float angle, float ratio, float near,
    					      float far, vec3 &camPos, vec3 & lookAtPos, vec3 & up);

    	void AddPlane(Loops::Core::Math::Plane& plane);
    	bool IsPointVisible(vec3 point);
    	bool IsSphereVisible(vec3 center, float rad);
    	bool IsBoxVisible(vec3 min, vec3 max);
    	int GetTotalPlanes();
    };
}
