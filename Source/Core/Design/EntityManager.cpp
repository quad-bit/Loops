#include "EntityManager.h"
#include "Entity.h"
#include "EntityHandle.h"

using namespace Loops::Core::ECS;

EntityManager* EntityManager::entityManagerInstance = nullptr;

EntityManager::EntityManager()
{
}

EntityManager * EntityManager::GetSingleton()
{
    if (entityManagerInstance == nullptr)
    {
        entityManagerInstance = new EntityManager();
    }
    return entityManagerInstance;
}

void EntityManager::Init()
{
}

void EntityManager::DeInit()
{
	entityList.clear();
	entityHandleList.clear();
}


Entity * EntityManager::CreateEntity()
{
    lastEntity++;
    Entity * obj = new Entity();
    obj->id = lastEntity;
	entityList.push_back(obj);
    return obj;
}

EntityHandle * EntityManager::CreateEntityHandle(Entity * obj, World * worldObj)
{
	EntityHandle * handle = new EntityHandle(obj, worldObj);
	entityHandleList.push_back(handle);

	return handle;
}

void EntityManager::DestroyEntity(Entity * entity)
{
	uint32_t pos = 0;
	for (uint32_t i = 0; i < entityList.size(); i++) 
	{
		if (entityList[i]->id == entity->id)
		{
			pos = i;
			break;
		}
    } 

	entityList.erase(entityList.begin() + pos);
	entityHandleList.erase(entityHandleList.begin() + pos);
}
