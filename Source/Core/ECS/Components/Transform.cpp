#include "Transform.h"
#include "EventBus.h"
#include "SceneChangeEvent.h"
#include "Entity.h"

void Transform::Init()
{
    localPosition = glm::vec3(0, 0, 0);
    localScale = glm::vec3(1, 1, 1);
    localEulerAngle = glm::vec3(0, 0, 0);

    globalPosition = glm::vec3(0, 0, 0);
    globalScale = glm::vec3(1, 1, 1);
    globalEulerAngle = glm::vec3(0, 0, 0);

    parent = nullptr;
    //parentNode = nullptr;
    nodeType = NODE_TYPE::TRANSFORM;

    // publish scene node creation event

    NodeAdditionEvent event;
    event.node = this;
    EventBus::GetInstance()->Publish(&event);
}

Transform::Transform()
{
    
}

Transform::Transform(Entity * entity)
{
    owner = entity;
    entityName = &owner->entityName; 
    type = COMPONENT_TYPE::TRANSFORM;
    Init();
}

Transform * Transform::GetParent()
{
    return parent;
}

void Transform::SetParent(Transform * transform)
{
    parent = transform;

    TreeModificationEvent event;
    event.mod = TreeModificationEvent::ModType::SET_PARENT;
    event.node = this;
    event.newParent = transform;

    EventBus::GetInstance()->Publish(&event);
}

const std::vector<Transform*>& Transform::GetChildren()
{
    return childrenList;
}

void Transform::Execute()
{
    std::cout << *entityName;
}
