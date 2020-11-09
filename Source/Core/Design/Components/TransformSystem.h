
#pragma once
#include "System.h"

class TransformSystem :	public System
{
public:
    virtual void Init() override;
    virtual void DeInit() override;
    virtual void Update(float dt) override;

    TransformSystem();
    virtual ~TransformSystem();
};
