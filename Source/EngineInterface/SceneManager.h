#pragma once
#include <InputManager.h>

class EntityHandle;
class Scriptable;
struct KeyInputEvent;
class Transform;

class SceneManager
{
private:
    EntityHandle * sceneRootEntityHandle;
    Transform * sceneRootTransform;
    
public:
    SceneManager();
    ~SceneManager();

    void HandleSceneControls(KeyInputEvent * inputEvent);
};
