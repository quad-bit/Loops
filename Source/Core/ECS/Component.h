#pragma once

#include <stdint.h>
#include <vector>

using namespace std;

struct ComponentCounter {
    static int counter;
};

template <typename ComponentType>
class Component
{
private:
    static int family;

public:

    static inline int Family()
    {
        if (family == -1)
            family = ComponentCounter::counter++;

        return family;
    }
};


template <typename ComponentType>
int Component<ComponentType>::family = -1;

//TODO: Need more understanding on the below function

template<typename ComponentType>
int GetComponentFamily()
{
    int family = Component<typename std::remove_const<ComponentType>::type>::Family();
    return family;
}

template<typename ComponentType>
void AssignFamily()
{
    Component<typename std::remove_const<ComponentType>::type>::Family();
}

