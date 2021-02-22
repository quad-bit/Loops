#include "SceneManagerScript.h"
#include "CameraController.h"
#include "PlayerHandlerScript.h"
#include <Camera.h>
#include <glm\glm.hpp>
#include <EntityHandle.h>
#include <Transform.h>

SceneManagerScript::SceneManagerScript() : Scriptable(false)
{
    scriptName = typeid(this).raw_name();

    camHandle0 = worldObj->CreateEntity();
    camHandle0->GetEntity()->entityName = "MainCamera";
    camHandle0->GetTransform()->SetLocalPosition(glm::vec3(0, 0, 10));
    camHandle0->GetTransform()->SetLocalEulerAngles(glm::vec3(0, 0, 0));

    Camera * camera = new Camera(camHandle0->GetTransform());
    camHandle0->AddComponent<Camera>(camera);

    ComponentHandle<Camera> test = camHandle0->GetComponent<Camera>();

    cameraController = new CameraController();
    camHandle0->AddComponent<Scriptable>(cameraController);

    mainObject = worldObj->FindEntity("mainObject");
    ASSERT_MSG_DEBUG(mainObject != nullptr, "Object not found");

    playerHandlerScript = new PlayerHandlerScript();
    mainObject->AddComponent<Scriptable>(playerHandlerScript);
}

SceneManagerScript::~SceneManagerScript()
{
    delete cameraController;
    worldObj->DestroyEntity(camHandle0);

    mainObject->RemoveComponent<Scriptable>(playerHandlerScript);
    delete playerHandlerScript;
    playerHandlerScript = NULL;
}

void SceneManagerScript::Init()
{
}

void SceneManagerScript::Update(float dt)
{
}

void SceneManagerScript::Render(float dt)
{
}

void SceneManagerScript::DeInit()
{
}
