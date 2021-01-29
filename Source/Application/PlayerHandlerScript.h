#pragma once

#include <Scriptable.h>

class EntityHandle;
class MeshRenderer;

class PlayerHandlerScript : public Scriptable
{
private:
    EntityHandle* camHandle0;
    EntityHandle* camHandle1;
    EntityHandle* playerHandle;
    EntityHandle* head;
    EntityHandle* torso;
    EntityHandle* leftArm;
    EntityHandle* rightArm;
    EntityHandle* rightLeg;
    EntityHandle* leftLeg;

    MeshRenderer * headMeshRenderer;
    MeshRenderer * torsoMeshRenderer;
    MeshRenderer * leftArmMeshRenderer;
    MeshRenderer * rightArmMeshRenderer;
    MeshRenderer * rightLegMeshRenderer;
    MeshRenderer * leftLegMeshRenderer;

public:
    PlayerHandlerScript();
    void Init() override;
    void Update(float dt) override;
    void DeInit() override;
    ~PlayerHandlerScript();
};