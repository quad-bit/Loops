
#pragma once
#include "System.h"
#pragma once
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
