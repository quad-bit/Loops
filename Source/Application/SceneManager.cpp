#include "SceneManager.h"
#include "EntityHandle.h"
#include "Scriptable.h"
#include "ECS_Setting.h"
#include "PlayerHandlerScript.h"
#include <EventBus.h>
#include <InputEvents.h>

SceneManager::SceneManager()
{
    scriptableParent = worldObj->CreateEntity();
    playerHandlerScript = new PlayerHandlerScript();
    scriptableParent->AddComponent<Scriptable>(playerHandlerScript);

    EventBus::GetInstance()->Subscribe<SceneManager, KeyInputEvent>(this, &SceneManager::HandleSceneControls);
}

SceneManager::~SceneManager()
{
    scriptableParent->RemoveComponent<Scriptable>(playerHandlerScript);
    delete playerHandlerScript;
    worldObj->DestroyEntity(scriptableParent);
}

void SceneManager::HandleSceneControls(KeyInputEvent * inputEvent)
{
    if (strcmp(inputEvent->keyname, "P") == 0)
    {
        if (inputEvent->keyState == KeyInputEvent::KEY_STATE::RELEASED)
        {
            if (appState == APP_STATE::NONE || appState == APP_STATE::STOPPED)
                appState = APP_STATE::STARTED;
            else if (appState == APP_STATE::RUNNING)
                appState = APP_STATE::STOPPED;
        }
    }
}
