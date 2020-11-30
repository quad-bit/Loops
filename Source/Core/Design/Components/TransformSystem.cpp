#include "TransformSystem.h"
#include "Transform.h"
#include "World.h"

void TransformSystem::UpdateTransform(Transform * transform)
{
    if (transform->GetParent() != nullptr)
    {
        transform->translationMat = glm::translate(transform->localPosition);
        transform->scaleMat = glm::scale(transform->localScale);

        glm::mat4 rotXMat = glm::rotate(transform->localEulerAngle.x, glm::vec3(1, 0, 0));
        glm::mat4 rotYMat = glm::rotate(transform->localEulerAngle.y, glm::vec3(0, 1, 0));
        glm::mat4 rotZMat = glm::rotate(transform->localEulerAngle.z, glm::vec3(0, 0, 1));

        transform->rotationMat = rotZMat * rotYMat * rotXMat;
        transform->localModelMatrix = transform->translationMat * transform->rotationMat * transform->scaleMat;
        transform->globalModelMatrix = transform->localModelMatrix;
    }
    else
    {
        transform->translationMat = glm::translate(transform->localPosition);
        transform->scaleMat = glm::scale(transform->localScale);

        glm::mat4 rotXMat = glm::rotate(transform->localEulerAngle.x, glm::vec3(1, 0, 0));
        glm::mat4 rotYMat = glm::rotate(transform->localEulerAngle.y, glm::vec3(0, 1, 0));
        glm::mat4 rotZMat = glm::rotate(transform->localEulerAngle.z, glm::vec3(0, 0, 1));

        transform->rotationMat = rotZMat * rotYMat * rotXMat;
        transform->localModelMatrix = transform->translationMat * transform->rotationMat * transform->scaleMat;
        transform->globalModelMatrix = transform->GetParent()->globalModelMatrix * transform->localModelMatrix;
    }
}

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
		ComponentHandle<Transform> transformHandle;
		worldObj->Unpack(entity, transformHandle);

        if (transformHandle->GetParent() != nullptr)
        {
            if (transformHandle->GetParent()->isTransformUpdated)
            {
                // update your tranformations
                UpdateTransform(transformHandle.GetComponent());
                transformHandle->isTransformUpdated = true;
            }
            else
            {
                // update parent transformation
                Transform * parent = transformHandle->GetParent();
                do
                {
                    UpdateTransform(parent);
                    parent->isTransformUpdated = true;
                    parent = parent->GetParent();
                }while (parent != nullptr);

                // update your tranformations
                UpdateTransform(transformHandle.GetComponent());
                transformHandle->isTransformUpdated = true;
            }
        }
	}
}

TransformSystem::TransformSystem()
{
    signature.AddComponent<Transform>();
}


TransformSystem::~TransformSystem()
{
}
