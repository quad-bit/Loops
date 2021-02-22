#pragma once

class EntityHandle;
class Scriptable;

class ApplicationController
{
private:
    EntityHandle* mainObject;
    Scriptable* sceneManagerScript;

public:
    void Init();
    void Update();
    void DeInit();

};
