#pragma once
#include <stdint.h>

namespace Loops::Core::ECS
{
    class Entity;

    template <typename ComponentType>
    class ComponentManager;

    template <typename ComponentType>
    class ComponentHandle
    {
    private:
        Entity * owner;
        ComponentManager<ComponentType> * componentManagerObj;
        ComponentType * componentType;

    public:
        ComponentHandle() {}
        ComponentHandle(ComponentManager<ComponentType>* componentManagerObj, Entity * owner, uint32_t componentIndex);
        void DestroyComponent();
        ComponentType *operator->() const { return componentType; }
    };
}

#include "ComponentManager.h"
using namespace Loops::Core::ECS;

template<typename ComponentType>
inline ComponentHandle<ComponentType>::ComponentHandle(ComponentManager<ComponentType>* componentManagerObj, Entity * owner, uint32_t componentIndex)
{
    this->owner = owner;
    this->componentManagerObj = componentManagerObj;
    this->componentType = componentManagerObj->GetComponent(owner);
}

template<typename ComponentType>
void ComponentHandle<ComponentType>::DestroyComponent()
{
    componentManagerObj->RemoveComponent(owner);
}
