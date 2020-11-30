#include "Transform.h"

Transform * Transform::GetParent()
{
    return parent;
}

void Transform::SetParent(Transform * transform)
{
    parent = transform;
}

const std::vector<Transform*>& Transform::GetChildren()
{
    return childrenList;
}
