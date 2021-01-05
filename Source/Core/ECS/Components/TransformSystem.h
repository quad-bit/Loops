
#pragma once
#include "System.h"

class Transform;

template <typename T>
class ComponentHandle;

class TransformSystem :	public System
{
private:
    void UpdateTransform(Transform * transform);
public:
    virtual void Init() override;
    virtual void DeInit() override;
    virtual void Update(float dt) override;

    TransformSystem();
    virtual ~TransformSystem();
};
