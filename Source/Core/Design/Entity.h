#pragma once
#include <iostream>

class Entity
{
public:
    uint32_t id;

    // To put it in maps
    friend bool operator<(const Entity &l, const Entity &r) { return l.id < r.id; }
};