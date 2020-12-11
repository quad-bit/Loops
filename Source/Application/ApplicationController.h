#pragma once

class EntityHandle;
class Scriptable;

class ApplicationController
{
private:
    EntityHandle* scriptableParent;
    Scriptable* playerHandlerScript;

public:
    void Init();
    void Update();
    void DeInit();

};
