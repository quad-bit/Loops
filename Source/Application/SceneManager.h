#pragma once

class EntityHandle;
class Scriptable;

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
};
