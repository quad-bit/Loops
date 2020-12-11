#include "ApplicationController.h"
#include <Scriptable.h>
#include <ECS_Setting.h>
#include "PlayerHandlerScript.h"
#include "EntityHandle.h"

void ApplicationController::Init()
{
    scriptableParent = worldObj->CreateEntity();
    playerHandlerScript = new PlayerHandlerScript();
    scriptableParent->AddComponent<Scriptable>(playerHandlerScript);
}

void ApplicationController::Update()
{

}

void ApplicationController::DeInit()
{
    scriptableParent->RemoveComponent<Scriptable>(playerHandlerScript);
    delete playerHandlerScript;
    playerHandlerScript = NULL;
    worldObj->DestroyEntity(scriptableParent);

}
