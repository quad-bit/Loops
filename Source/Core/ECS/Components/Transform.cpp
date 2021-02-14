#include "Transform.h"
#include "EventBus.h"
#include "SceneChangeEvent.h"
#include "Entity.h"


void Transform::Init()
{
    up = glm::vec3(0, 1, 0);
    right = glm::vec3(1, 0, 0);
    forward = glm::vec3(0, 0, -1);

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
}

void Transform::UpdateLocalParams()
{
    this->translationMat = glm::translate(this->localPosition);
    this->scaleMat = glm::scale(this->localScale);

    glm::mat4 rotXMat = glm::rotate(this->localEulerAngle.x, glm::vec3(1, 0, 0));
    glm::mat4 rotYMat = glm::rotate(this->localEulerAngle.y, glm::vec3(0, 1, 0));
    glm::mat4 rotZMat = glm::rotate(this->localEulerAngle.z, glm::vec3(0, 0, 1));

    this->rotationMat = rotZMat * rotYMat * rotXMat;

    glm::vec4 temp = rotXMat * Vec3ToVec4_0(forward);
    //temp = glm::normalize(temp);
    forward =  Vec4ToVec3(temp);

    temp = rotationMat * Vec3ToVec4_0(right);
    temp = glm::normalize(temp);
    right = Vec4ToVec3(temp);

    temp = rotationMat * Vec3ToVec4_0(up);
    temp = glm::normalize(temp);
    up = Vec4ToVec3(temp);

    this->localModelMatrix = this->translationMat * this->rotationMat * this->scaleMat;
}

const std::vector<Transform*>& Transform::GetChildren()
{
    return childrenList;
}

glm::vec3 Transform::GetForward()
{
    return ( forward );
}

glm::vec3 Transform::GetUp()
{
    return up;
}

glm::vec3 Transform::GetRight()
{
    return right;
}

glm::vec3 Transform::GetLocalPosition()
{
    return localPosition;
}

glm::vec3 Transform::GetLocalEulerAngles()
{
    return localEulerAngle;
}

glm::vec3 Transform::GetLocalScale()
{
    return localScale;
}

glm::mat4 Transform::GetLocalModelMatrix()
{
    return localModelMatrix;
}

glm::vec3 Transform::GetGlobalPosition()
{
    ASSERT_MSG(0, "Yet to be implemented");
    return globalPosition;
}

glm::vec3 Transform::GetGlobalEulerAngles()
{
    ASSERT_MSG(0, "Yet to be implemented");
    return globalEulerAngle;
}

glm::vec3 Transform::GetGlobalScale()
{
    ASSERT_MSG(0, "Yet to be implemented");
    return globalScale;
}

glm::mat4 Transform::GetGlobalModelMatrix()
{
    //ASSERT_MSG(0, "Yet to be implemented");
    return globalModelMatrix;
}

void Transform::SetLocalPosition(const glm::vec3 & pos)
{
    localPosition = pos;
    UpdateLocalParams();
}

void Transform::SetLocalEulerAngles(const glm::vec3 & angle)
{
    localEulerAngle = angle;
    UpdateLocalParams();
}

void Transform::SetLocalScale(const glm::vec3 & scale)
{
    localScale = scale;
    UpdateLocalParams();
}

void Transform::SetLocalModelMatrix(const glm::mat4 & mat)
{
    localModelMatrix = mat;
}

void Transform::SetGlobalPosition(const glm::vec3 & pos)
{
    ASSERT_MSG(0, "Yet to be implemented");
    globalPosition = pos;
}

void Transform::SetGlobalEulerAngles(const glm::vec3 & angle)
{
    ASSERT_MSG(0, "Yet to be implemented");
    globalEulerAngle = angle;
}

void Transform::SetGlobalScale(const glm::vec3 & scale)
{
    ASSERT_MSG(0, "Yet to be implemented");
    globalScale = scale;
}

void Transform::SetGlobalModelMatrix(const glm::mat4 & mat)
{
    //ASSERT_MSG(0, "Yet to be implemented");
    globalModelMatrix = mat;
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

    //UpdateGlobalParams();

    // Push the global matrix into the transformation Matrix stack
    //this->localModelMatrix = accumulateMatrix * this->globalModelMatrix;

    //UpdateLocalParams();
    
    UpdateLocalParams();
    //this->globalModelMatrix = accumulateMatrix * this->localModelMatrix;
    this->SetGlobalModelMatrix(accumulateMatrix * this->GetLocalModelMatrix());

    SceneTreeUtil::matrixList.push_back(this->GetLocalModelMatrix());
}

void Transform::Exit()
{
    // pop the transformation stack
    //PLOGD << "exit " << *entityName;
    //SceneTreeUtil::nameList.pop_back();
    SceneTreeUtil::matrixList.pop_back();
}
