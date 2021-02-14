#pragma once

#include <Scriptable.h>

class EntityHandle;
class MeshRenderer;
class CameraController;

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

    Scriptable * cameraController;

    float prevAngle, currentAngle;

public:
    PlayerHandlerScript();
    void Init() override;
    void Update(float dt) override;
    void Render(float dt) override;
    void DeInit() override;
    virtual ~PlayerHandlerScript();
};