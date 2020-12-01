#pragma once
#include <InputManager.h>

class EntityHandle;
class Scriptable;
struct KeyInputEvent;

class SceneManager
{
private:
    EntityHandle* scriptableParent;
    Scriptable* playerHandlerScript;
    /*EntityHandle* Player;
    EntityHandle* head;
    EntityHandle* torso;
    EntityHandle* leftArm;
    EntityHandle* rightArm;
    EntityHandle* rightLeg;
    EntityHandle* leftLeg;
*/
public:
    SceneManager();
    ~SceneManager();

    void HandleSceneControls(KeyInputEvent * inputEvent);
};
