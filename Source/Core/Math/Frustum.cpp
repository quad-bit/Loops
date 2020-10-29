#include "Frustum.h"

using namespace Loops::Core::Math;
using namespace Loops::Core::Utils::Container;

Frustum::Frustum()
{
}

Frustum::~Frustum()
{
}

void Frustum::CalculateFrustum(float angle, float ratio, float near, float far, vec3 & camPos, vec3 & lookAtPos, vec3 & up)
{
	vec3 xVec, yVec, zVec, vecN, vecF;
	vec3 nearTopLeft, nearTopRight, nearBottomLeft, nearBottomRight;
	vec3 farTopLeft, farTopRight, farBottomRight, farBottomLeft;
	float radians = (float)glm::tan(glm::degrees(angle) * 0.5f);
	float nearH = near * radians;
	float nearW = nearH * ratio;
	float farH = far * radians;
	float farW = farH * ratio;

	zVec = camPos - lookAtPos;
	zVec = glm::normalize(zVec);

	xVec = glm::cross(up, zVec);
	xVec = glm::normalize(xVec);

	yVec = glm::cross(zVec, xVec);

	vecN = camPos - zVec * near;
	vecF = camPos - zVec * far;

	nearTopLeft = vecN + yVec * nearH - xVec * nearW;
	nearTopRight = vecN + yVec * nearH + xVec * nearW;
	nearBottomLeft = vecN - yVec * nearH - xVec * nearW;
	nearTopRight = vecN - yVec * nearH + xVec * nearW;

	farTopLeft = vecF + yVec * farH - xVec * farW;
	farTopRight = vecF + yVec * farH + xVec * farW;
	farBottomLeft = vecF - yVec * farH - xVec * farW;
	farTopRight = vecF - yVec * farH + xVec * farW;

	frustum.Clear();

	Loops::Core::Math::Plane planeObj;

	planeObj.Create(nearTopRight, nearTopLeft, farTopLeft);
	AddPlane(planeObj);

	planeObj.Create(nearBottomLeft, nearBottomRight, farBottomRight);
	AddPlane(planeObj);

	planeObj.Create(nearTopLeft, nearBottomLeft, farBottomLeft);
	AddPlane(planeObj);

	planeObj.Create(nearBottomRight, nearTopRight, farBottomRight);
	AddPlane(planeObj);

	planeObj.Create(nearTopLeft, nearTopRight, nearBottomRight);
	AddPlane(planeObj);

	planeObj.Create(farTopRight, farTopLeft, farBottomLeft);
	AddPlane(planeObj);
}

void Frustum::AddPlane(Loops::Core::Math::Plane & plane)
{
	frustum.Push(plane);
}

bool Frustum::IsPointVisible(vec3 point)
{
	for (int i = 0; i < frustum.GetSize(); i++)
	{
		if (frustum[i].GetDistance(point) < 0)
		{
			return false;
		}
	}
	return true;
}

bool Frustum::IsSphereVisible(vec3 center, float rad)
{
	float distance = 0;

	for (int i = 0; i < frustum.GetSize(); i++)
	{
		distance = frustum[i].GetDistance(center);
		if (distance < -rad)
			return false;
	}
	return true;
}

bool Frustum::IsBoxVisible(vec3 min, vec3 max)
{
	if (IsPointVisible(min)) return true;
	if (IsPointVisible(vec3(max.x, min.y, min.z))) return true;
	if (IsPointVisible(vec3(min.x, max.y, min.z))) return true;
	if (IsPointVisible(vec3(max.x, max.y, min.z))) return true;
	if (IsPointVisible(vec3(min.x, min.y, max.z))) return true;
	if (IsPointVisible(vec3(max.x, min.y, max.z))) return true;
	if (IsPointVisible(vec3(min.x, max.y, max.z))) return true;
	if (IsPointVisible(max)) return true;

	return false;
}

int Frustum::GetTotalPlanes()
{
	return frustum.GetSize();
}
