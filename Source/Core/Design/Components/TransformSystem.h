
#pragma once
#include "System.h"

namespace Loops::Core::ECS
{
    class TransformSystem :	public System
    {
    public:
    	virtual void Init() override;
    	virtual void DeInit() override;
    	virtual void Update(float dt) override;

    	TransformSystem();
    	virtual ~TransformSystem();
    };
}
