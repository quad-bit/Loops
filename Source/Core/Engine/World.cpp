#include "World.h"
#include "System.h"
#include "EntityManager.h"

EntityHandle* World::CreateEntity()
{
    Entity* obj = EntityManager::GetSingleton()->CreateEntity();
    EntityHandle * handle = EntityManager::GetSingleton()->CreateEntityHandle(obj, this);

    ComponentMask mask;
    entityMasks.insert({ *obj, mask });

    return handle;
}

void World::UpdateEntityMask(Entity * entity, ComponentMask oldMask)
{
    ComponentMask newMask = entityMasks[*entity];
    // TODO : Complete the system integration

    for (auto &system : systemList) {
        ComponentMask systemSignature = system->GetSignature();
        if (newMask.isNewMatch(oldMask, systemSignature)) {
            // We match but didn't match before
            system->RegisterEntity(entity);
        }
        else if (newMask.isNoLongerMatched(oldMask, systemSignature)) {
            system->UnRegisterEntity(entity);
        }
    }
}

void World::AddSystem(System * system)
{
    systemList.push_back(system);
    system->RegisterWorld(this);
}

void World::Update(float dt)
{
    for (auto system : systemList)
    {
        system->Update(dt);
    }
}

void World::Render()
{
    for (auto system : systemList)
    {
        system->Render();
    }
}

void World::DeInit()
{
    for (auto system : systemList)
    {
        system->DeInit();
    }
}

void World::Init()
{
    for (auto system : systemList)
    {
        system->Init();
    }
}

void World::DestroyEntity(Entity* entityObj)
{
    for (auto system : systemList)
    {
        system->UnRegisterEntity(entityObj);
    }
}