#pragma once
#include "Component.h"
#include <string>

class Entity;
class Scriptable : public Component<Scriptable>
{

public:
    Entity * entity;
    string scriptName;

    virtual void Init() {};
    virtual void Update(float dt) {};
    virtual void DeInit() {};
    
    virtual ~Scriptable() {};
};