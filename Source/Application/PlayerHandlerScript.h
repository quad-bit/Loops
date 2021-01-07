#pragma once

#include <Scriptable.h>

class EntityHandle;

class PlayerHandlerScript : public Scriptable
{
private:
    EntityHandle* camHandle;
    EntityHandle* playerHandle;
    EntityHandle* head;
    EntityHandle* torso;
    EntityHandle* leftArm;
    EntityHandle* rightArm;
    EntityHandle* rightLeg;
    EntityHandle* leftLeg;

public:
    PlayerHandlerScript();
    void Init() override;
    void Update(float dt) override;
    void DeInit() override;
    ~PlayerHandlerScript();
};