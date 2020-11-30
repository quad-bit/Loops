#pragma once
#include <iostream>
class Transform;
class Entity
{
public:
    uint32_t id;

    Transform * transform;
    // To put it in maps
    friend bool operator<(const Entity &l, const Entity &r) { return l.id < r.id; }

    ~Entity()
    {
        //std::cout << "entity deleted \n";
    }
};
