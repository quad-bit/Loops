
#pragma once
#include "System.h"

class Scriptable;

class ScriptableSystem : public System
{
private:
    
public:
    virtual void Init() override;
    virtual void DeInit() override;
    virtual void Update(float dt) override;

    ScriptableSystem();
    virtual ~ScriptableSystem();
};
