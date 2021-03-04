#pragma once

#include <Scriptable.h>

class EntityHandle;
class MeshRenderer;
class CameraController;
class Light;

class SceneManagerScript : public Scriptable
{
private:
    EntityHandle* mainObject;

    EntityHandle* camHandle0;
    EntityHandle* camHandle1;
    Scriptable * cameraController;
    
    EntityHandle* playerHandle;
    Scriptable* playerHandlerScript;
    
    EntityHandle* lightHandle;
    Light * lightComponent;
    MeshRenderer * lightDebugRenderer;
    
    EntityHandle* floorHandle, *wallHandle;
    MeshRenderer * floorRenderer, *wallRenderer;

    float prevAngle, currentAngle;

public:
    SceneManagerScript();
    virtual ~SceneManagerScript();

    void Init() override;
    void Update(float dt) override;
    void Render(float dt) override;
    void DeInit() override;
};
