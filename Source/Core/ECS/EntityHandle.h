#pragma once
#include "World.h"

#include <iostream>

class Entity;
class World;
class Transform;

template< typename ComponentType>
class ComponentHandle;

class EntityHandle
{
private:
    Entity * entityObj;
    World * worldObj;
    //Transform * transform;

public:
    EntityHandle(Entity * entityObj, World * worldObj) {
        this->entityObj = entityObj;
        //this->transform = entityObj->transform;
        this->worldObj = worldObj;
    }

    void Destroy()
    {
        worldObj->DestroyEntity(entityObj);
    }

    Entity * GetEntity() { return entityObj; }
    Transform * GetTransform(){ return entityObj->transform; }

    template<typename ComponentType>
    void AddComponent(ComponentType* componentType);

    template<typename ComponentType>
    void RemoveComponent(ComponentType* componentType);

    template<typename ComponentType>
    ComponentHandle<ComponentType> GetComponent();

    ~EntityHandle()
    {
        //std::cout << "entity handle deleted \n";
    }

};

#include "Entity.h"
#include "ComponentHandle.h"
#include "Camera.h"
#include "ComponentAdditionEvent.h"
#include "EventBus.h"
#include "MeshRenderer.h"

template<typename ComponentType>
inline void EntityHandle::AddComponent(ComponentType * componentType)
{
    worldObj->AddComponent<ComponentType>(componentType, entityObj);
}

template<>
inline void EntityHandle::AddComponent(Camera * componentType)
{
    worldObj->AddComponent<Camera>(componentType, entityObj);
    
    CameraAdditionEvent evt;
    evt.cam = componentType;
    EventBus::GetInstance()->Publish(&evt);
}

template<>
inline void EntityHandle::AddComponent(MeshRenderer * componentType)
{
    worldObj->AddComponent<MeshRenderer>(componentType, entityObj);

    MeshRendererAdditionEvent evt;
    evt.renderer = componentType;
    EventBus::GetInstance()->Publish(&evt);
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

