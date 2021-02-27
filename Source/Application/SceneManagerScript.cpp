#include "SceneManagerScript.h"
#include "CameraController.h"
#include "PlayerHandlerScript.h"
#include <Camera.h>
#include <glm\glm.hpp>
#include <EntityHandle.h>
#include <Transform.h>
#include <Light.h>
#include <bitset>
#include <AttributeHelper.h>
#include <MeshFactory.h>
#include <MaterialFactory.h>

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

    lightHandle = worldObj->CreateEntity("light");
    ComponentHandle<Transform> lightTrfHandle = lightHandle->GetComponent<Transform>();
    lightTrfHandle->SetLocalPosition(glm::vec3(3, 3, 0));
    lightTrfHandle->SetLocalEulerAngles(glm::vec3(0, 0, 0));

    lightComponent = new Light(lightTrfHandle.GetComponent());
    lightHandle->AddComponent<Light>(lightComponent);

    Material * colMat;

    {
        std::bitset<(unsigned int)ATTRIBUTES::NUM_ATTRIBUTES> req;
        req.set((unsigned int)ATTRIBUTES::POSITION);
        req.set((unsigned int)ATTRIBUTES::COLOR);

        PrimtiveType * prim = new PrimtiveType{ PrimtiveType::TOPOLOGY_TRIANGLE_LIST };
        MeshInfo meshInfo{};
        meshInfo.attribMaskReq = req;
        meshInfo.bufferPerAttribRequired = false;
        meshInfo.isIndexed = true;
        meshInfo.isPrimitiveRestartEnabled = false;
        meshInfo.primitive = prim;
        MESH_TYPE meshType = MESH_TYPE::CUBE;

        Mesh * mesh = MeshFactory::GetInstance()->CreateMesh(&meshInfo, &meshType);
        lightHandle->AddComponent<Mesh>(mesh);

        ShaderDescription shaders[2];
        shaders[0].type = ShaderType::VERTEX;
        shaders[0].shaderName = "PC.vert";

        shaders[1].type = ShaderType::FRAGMENT;
        shaders[1].shaderName = "Color.frag";

        colMat = MaterialFactory::GetInstance()->CreateMaterial(shaders, 2, mesh->componentId);
        lightHandle->AddComponent<Material>(colMat);

        lightDebugRenderer = new MeshRenderer(mesh, colMat, lightTrfHandle.GetComponent());
        lightHandle->AddComponent<MeshRenderer>(lightDebugRenderer);
    }
}

SceneManagerScript::~SceneManagerScript()
{
    {
        ComponentHandle<Mesh> mesh = lightHandle->GetComponent<Mesh>();
        MeshFactory::GetInstance()->DestroyMesh(mesh->componentId);
        mesh.DestroyComponent();
        //torso->RemoveComponent<Mesh>(torsoMesh.GetComponent());
        //delete torsoMesh.GetComponent(); //TODO  fix this get component function
        ComponentHandle<Material> mat = lightHandle->GetComponent<Material>();
        mat.DestroyComponent();
    }

    lightHandle->RemoveComponent<Light>(lightComponent);
    worldObj->DestroyEntity(lightHandle);

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
