#pragma once
#include "Component.h"
#include <string>

class Entity;
class Scriptable : public Component<Scriptable>
{
private:
    Scriptable() = delete;

public:
    Entity * entity;
    string scriptName;
    bool runInEditMode = false;

    virtual void Init() {};
    virtual void Update(float dt) {};
    virtual void DeInit() {};
    Scriptable(bool runInEditMode = false) { this->runInEditMode = runInEditMode; }
    virtual ~Scriptable() {};
};