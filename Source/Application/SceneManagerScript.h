#pragma once

#include <Scriptable.h>

class EntityHandle;
class MeshRenderer;
class CameraController;

class SceneManagerScript : public Scriptable
{
private:
    EntityHandle* mainObject;
    Scriptable* playerHandlerScript;

    EntityHandle* camHandle0;
    EntityHandle* camHandle1;
    EntityHandle* playerHandle;

    Scriptable * cameraController;

    float prevAngle, currentAngle;

public:
    SceneManagerScript();
    virtual ~SceneManagerScript();

    void Init() override;
    void Update(float dt) override;
    void Render(float dt) override;
    void DeInit() override;
};
