#include "Transform.h"
#include "EventBus.h"
#include "SceneChangeEvent.h"
#include "Entity.h"

#define Vec4ToVec3(a) glm::vec3(a.x, a.y, a.z)
#define Vec3ToVec4_1(a) glm::vec4(a.x, a.y, a.z, 1.0f)
#define Vec3ToVec4_0(a) glm::vec4(a.x, a.y, a.z, 0.0f)

void Transform::Init()
{
    localPosition = glm::vec3(0, 0, 0);
    localScale = glm::vec3(1, 1, 1);
    localEulerAngle = glm::vec3(0, 0, 0);
    localModelMatrix = glm::identity<glm::mat4>();

    globalPosition = glm::vec3(0, 0, 0);
    globalScale = glm::vec3(1, 1, 1);
    globalEulerAngle = glm::vec3(0, 0, 0);
    globalModelMatrix = glm::identity<glm::mat4>();
    UpdateGlobalParams();

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

void Transform::UpdateGlobalParams()
{
    this->translationMat = glm::translate(this->globalPosition);
    this->scaleMat = glm::scale(this->globalScale);

    glm::mat4 rotXMat = glm::rotate(this->globalEulerAngle.x, glm::vec3(1, 0, 0));
    glm::mat4 rotYMat = glm::rotate(this->globalEulerAngle.y, glm::vec3(0, 1, 0));
    glm::mat4 rotZMat = glm::rotate(this->globalEulerAngle.z, glm::vec3(0, 0, 1));

    this->rotationMat = rotZMat * rotYMat * rotXMat;
    this->globalModelMatrix = this->translationMat * this->rotationMat * this->scaleMat;
    //this->localModelMatrix = this->GetParent()->localModelMatrix * this->globalModelMatrix;
}

void Transform::UpdateLocalParams()
{
    glm::vec4 localPosition = this->localModelMatrix * Vec3ToVec4_1(this->globalPosition);
    this->localPosition = Vec4ToVec3(localPosition);
    
    // not sure about these
    {
        glm::vec4 localEulerAngles = this->localModelMatrix * Vec3ToVec4_1(this->globalEulerAngle);
        //this->localEulerAngle = Vec4ToVec3(localEulerAngles);

        glm::vec4 localScale = this->localModelMatrix * Vec3ToVec4_1(this->globalScale);
        //this->localScale = Vec4ToVec3(localScale);
    }
}

const std::vector<Transform*>& Transform::GetChildren()
{
    return childrenList;
}

void Transform::Entry()
{
    //SceneTreeUtil::nameList.push_back(*entityName);
    //PLOGD << "entry " << *entityName;

    glm::mat4 accumulateMatrix = glm::identity<glm::mat4>();
    for each(auto mat in SceneTreeUtil::matrixList)
    {
        accumulateMatrix = mat * accumulateMatrix;
    }

    UpdateGlobalParams();

    // Push the global matrix into the transformation Matrix stack
    this->localModelMatrix = accumulateMatrix * this->globalModelMatrix;

    UpdateLocalParams();
    
    SceneTreeUtil::matrixList.push_back(this->globalModelMatrix);
}

void Transform::Exit()
{
    // pop the transformation stack
    //PLOGD << "exit " << *entityName;
    //SceneTreeUtil::nameList.pop_back();
    SceneTreeUtil::matrixList.pop_back();
}
