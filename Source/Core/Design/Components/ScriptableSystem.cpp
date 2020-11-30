#include "ScriptableSystem.h"
#include "Scriptable.h"
#include "World.h"
#include "ECS_Setting.h"
#include "ComponentHandle.h"

void ScriptableSystem::Init()
{
}

void ScriptableSystem::DeInit()
{
}

void ScriptableSystem::Update(float dt)
{
    switch (appState)
    {
    case APP_STATE::STARTED:
        for (auto & entity : registeredEntities)
        {
            ComponentHandle<Scriptable> scriptableHandle;
            worldObj->Unpack(entity, scriptableHandle);

            scriptableHandle->Init();
        }
        appState = APP_STATE::RUNNING;
        break;

    case APP_STATE::RUNNING:
        for (auto & entity : registeredEntities)
        {
            ComponentHandle<Scriptable> scriptableHandle;
            worldObj->Unpack(entity, scriptableHandle);

            scriptableHandle->Update(dt);
        }
        break;

    case APP_STATE::PAUSED:
        break;

    case APP_STATE::STOPPED:
        for (auto & entity : registeredEntities)
        {
            ComponentHandle<Scriptable> scriptableHandle;
            worldObj->Unpack(entity, scriptableHandle);

            scriptableHandle->DeInit();
        }
        appState = APP_STATE::NONE;
        break;

    case APP_STATE::NONE:
        break;

    default:
        break;
    }
}

ScriptableSystem::ScriptableSystem()
{
    signature.AddComponent<Scriptable>();
}

ScriptableSystem::~ScriptableSystem()
{
}
