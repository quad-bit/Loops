#pragma once
#include <iostream>
//#include <string>

class Transform;
class Entity
{
public:
    uint32_t id;

    Transform * transform;
    const char * entityName;
    const char * entityTag;
    const char * entityLayer;
    bool isStatic;

    // To put it in maps
    friend bool operator<(const Entity &l, const Entity &r) { return l.id < r.id; }

    ~Entity()
    {
    }
};
