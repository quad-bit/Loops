#include "SceneManager.h"
#include "EntityHandle.h"
#include "Scriptable.h"
#include "ECS_Setting.h"
#include "PlayerHandlerScript.h"

SceneManager::SceneManager()
{
    scriptableParent = worldObj->CreateEntity();
    playerHandlerScript = new PlayerHandlerScript();
    scriptableParent->AddComponent<Scriptable>(playerHandlerScript);
}

SceneManager::~SceneManager()
{
    scriptableParent->RemoveComponent<Scriptable>(playerHandlerScript);
    delete playerHandlerScript;
    scriptableParent->Destroy();
    worldObj->DestroyEntity(scriptableParent->GetEntity());
}
