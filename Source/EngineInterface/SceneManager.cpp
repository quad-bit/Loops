#include "SceneManager.h"
#include "EntityHandle.h"
#include "Scriptable.h"
#include "ECS_Setting.h"
//#include "PlayerHandlerScript.h"
#include <EventBus.h>
#include <InputEvents.h>
#include <Transform.h>
#include <SceneGraphManager.h>

SceneManager::SceneManager()
{
    sceneRootEntityHandle = worldObj->CreateEntity();
    sceneRootTransform = sceneRootEntityHandle->GetEntity()->transform;
    sceneRootEntityHandle->GetEntity()->entityName = "sceneRoot";

    SceneGraphManager::GetInstance()->Init(sceneRootTransform);

    

    EventBus::GetInstance()->Subscribe<SceneManager, KeyInputEvent>(this, &SceneManager::HandleSceneControls);
}

SceneManager::~SceneManager()
{
    /*scriptableParent->RemoveComponent<Scriptable>(playerHandlerScript);
    delete playerHandlerScript;
    playerHandlerScript = NULL;
    worldObj->DestroyEntity(scriptableParent);
*/
    SceneGraphManager::GetInstance()->DeInit();
    delete SceneGraphManager::GetInstance();

    worldObj->DestroyEntity(sceneRootEntityHandle);
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

    if (strcmp(inputEvent->keyname, "T") == 0)
    {
        if (inputEvent->keyState == KeyInputEvent::KEY_STATE::RELEASED)
        {
            SceneGraphManager::GetInstance()->Update(0.0f);
        }
    }
}
