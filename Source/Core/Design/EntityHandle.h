#pragma once
#include "World.h"

class Entity;
class World;

template< typename ComponentType>
class ComponentHandle;

class EntityHandle
{
private:
    Entity * entityObj;
    World * worldObj;

public:
    EntityHandle(Entity * entityObj, World * worldObj) {
        this->entityObj = entityObj;
        this->worldObj = worldObj;
    }

    void Destroy()
    {
        worldObj->DestroyEntity(entityObj);
    }

    Entity * GetEntityHandle() { return entityObj; }

    template<typename ComponentType>
    void AddComponent(ComponentType* componentType);

    template<typename ComponentType>
    void RemoveComponent(ComponentType* componentType);

    template<typename ComponentType>
    ComponentHandle<ComponentType> GetComponent();
    
};

#include "Entity.h"
#include "ComponentHandle.h"

template<typename ComponentType>
inline void EntityHandle::AddComponent(ComponentType * componentType)
{
    worldObj->AddComponent<ComponentType>(componentType, entityObj);
}

template<typename ComponentType>
inline void EntityHandle::RemoveComponent(ComponentType * componentType)
{
    worldObj->RemoveComponent<ComponentType>(componentType, entityObj);
}

template<typename ComponentType>
inline ComponentHandle<ComponentType> EntityHandle::GetComponent()
{
    ComponentHandle<ComponentType> componentHandle;  //TODO : Write UNPACK
    worldObj->Unpack(entityObj, componentHandle);
    return componentHandle;
}

