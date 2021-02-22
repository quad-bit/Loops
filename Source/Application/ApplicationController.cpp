#include "ApplicationController.h"
#include <Scriptable.h>
#include <ECS_Setting.h>
#include "SceneManagerScript.h"
#include "EntityHandle.h"

void ApplicationController::Init()
{
    mainObject = worldObj->CreateEntity();
    mainObject->GetEntity()->entityName = "mainObject";
    sceneManagerScript = new SceneManagerScript();
    mainObject->AddComponent<Scriptable>(sceneManagerScript);
}

void ApplicationController::Update()
{

}

void ApplicationController::DeInit()
{
    mainObject->RemoveComponent<Scriptable>(sceneManagerScript);
    delete sceneManagerScript;
    sceneManagerScript = NULL;
    worldObj->DestroyEntity(mainObject);
}
