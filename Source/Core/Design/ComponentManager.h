#pragma once

#include <map>
#include <array>
#include <functional>
#include "ECS_Setting.h"

using namespace std;

class EntityMap;
class Entity;

template <typename ComponentType>
class ComponentHandle;

using ComponentIndex = uint32_t;

template <typename ComponentType>
class ComponentData
{
public:
    uint32_t size = 1;
    std::array<ComponentType*, MAX_NUM_OF_UNIQUE_COMPONENTS>* data;
};

class BaseComponentManager {
public:
    BaseComponentManager() = default;
    virtual ~BaseComponentManager() = default;
    BaseComponentManager(const BaseComponentManager &) = default;
    BaseComponentManager &operator=(const BaseComponentManager &) = default;
    BaseComponentManager(BaseComponentManager &&) = default;
    BaseComponentManager &operator=(BaseComponentManager &&) = default;
};

template <typename ComponentType>
class ComponentManager : public BaseComponentManager
{
private:
    ComponentData<ComponentType> componentDataObj;
    EntityMap * entityMapObj;

public:

    using LookupType = ComponentType;

    ComponentManager();
    ComponentIndex AddComponent(ComponentType * componentType, Entity * entity);
    void RemoveComponent(Entity * entity);
    ComponentType* GetComponent(Entity * entity);
    ComponentHandle<ComponentType>* GetComponentHandle(Entity * entity);
    void iterateAll(std::function<void(ComponentType* componentType)> lambda);

    ~ComponentManager();
};

#include "Entity.h"
#include "EntityMap.h"
#include "ComponentHandle.h"

template<typename ComponentType>
inline ComponentManager<ComponentType>::ComponentManager()
{
    componentDataObj.data = static_cast<std::array<ComponentType*, MAX_NUM_OF_UNIQUE_COMPONENTS>* >(malloc(sizeof(ComponentType) * MAX_NUM_OF_UNIQUE_COMPONENTS));
    entityMapObj = new EntityMap();
}

template<typename ComponentType>
inline ComponentManager<ComponentType>::~ComponentManager()
{
    delete entityMapObj;
    std::cout << "Deleting component manager\n";
}

template<typename ComponentType>
inline ComponentIndex ComponentManager<ComponentType>::AddComponent(ComponentType * componentType, Entity * entity)
{
    ComponentIndex ind = componentDataObj.size;
    componentDataObj.data->at(ind) = componentType;

    entityMapObj->AddToMap(entity, &ind);
    componentDataObj.size++;

    return ind;
}

template<typename ComponentType>
inline void ComponentManager<ComponentType>::RemoveComponent(Entity * e)
{
    ComponentIndex ind = entityMapObj->GetComponentIndex(e);
    ComponentIndex lastIndex = componentDataObj.size - 1;

    componentDataObj.data->at(ind) = componentDataObj.data->at(lastIndex);

    Entity lastEntity = entityMapObj->GetEntity(&lastIndex);

    entityMapObj->RemoveFromMap(e);
    entityMapObj->UpdateMap(&lastEntity, &ind);

    componentDataObj.size--;
}

template<typename ComponentType>
inline ComponentType* ComponentManager<ComponentType>::GetComponent(Entity * entity)
{
    ComponentIndex ind = entityMapObj->GetComponentIndex(entity);
    return componentDataObj.data->at(ind);
}

template<typename ComponentType>
inline ComponentHandle<ComponentType>* ComponentManager<ComponentType>::GetComponentHandle(Entity * entity)
{
    ComponentIndex ind = entityMapObj->GetComponentIndex(entity);
    ComponentHandle<ComponentType>* handle = new ComponentHandle<ComponentType>(this, entity, ind);

    return handle;
}

template<typename ComponentType>
inline void ComponentManager<ComponentType>::iterateAll(std::function<void(ComponentType* componentType)> lambda)
{
    for (int i = 1; i<componentDataObj.size; i++) {
        lambda(componentDataObj.data[i]);
    }

    /*

    componentManager.iterateAll([](positionComponent c){
    c.position += c.velocity;
    c.velocity += c.acceleration;
    }

    */
}
