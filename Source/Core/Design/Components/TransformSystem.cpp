#include "TransformSystem.h"
#include "Transform.h"
#include "World.h"

void TransformSystem::Init()
{
}

void TransformSystem::DeInit()
{
}

void TransformSystem::Update(float dt)
{
	for (auto & entity : registeredEntities)
	{
		ComponentHandle<Transform> transformObj;
		worldObj->Unpack(entity, transformObj);

		transformObj->translationMat = glm::translate(transformObj->position);
		transformObj->scaleMat = glm::scale(transformObj->scale);

		glm::mat4 rotXMat = glm::rotate(transformObj->eulerAngles.x, glm::vec3(1, 0, 0));
		glm::mat4 rotYMat = glm::rotate(transformObj->eulerAngles.y, glm::vec3(0, 1, 0));
		glm::mat4 rotZMat = glm::rotate(transformObj->eulerAngles.z, glm::vec3(0, 0, 1));

		transformObj->rotationMat = rotZMat * rotYMat * rotXMat;
		transformObj->modelMat = transformObj->translationMat * transformObj->rotationMat * transformObj->scaleMat;
	}
}

TransformSystem::TransformSystem()
{
}


TransformSystem::~TransformSystem()
{
}
