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

#define debugMeshForLight 0

SceneManagerScript::SceneManagerScript() : Scriptable(false)
{
    scriptName = typeid(this).raw_name();

    camHandle0 = worldObj->CreateEntity();
    camHandle0->GetEntity()->entityName = "MainCamera";
    camHandle0->GetTransform()->SetLocalPosition(glm::vec3(0, 15, 30));
    camHandle0->GetTransform()->SetLocalEulerAngles(glm::vec3(glm::radians(-19.0f), 0, 0));

    Camera * camera = new Camera(camHandle0->GetTransform());
    camHandle0->AddComponent<Camera>(camera);
    
    cameraController = new CameraController();
    camHandle0->AddComponent<Scriptable>(cameraController);

    mainObject = worldObj->FindEntity("mainObject");
    ASSERT_MSG_DEBUG(mainObject != nullptr, "Object not found");

    playerHandlerScript = new PlayerHandlerScript();
    mainObject->AddComponent<Scriptable>(playerHandlerScript);

    lightHandle = worldObj->CreateEntity("light");
    ComponentHandle<Transform> lightTrfHandle = lightHandle->GetComponent<Transform>();
    lightTrfHandle->SetLocalPosition(glm::vec3(15, 15, 0));
    lightTrfHandle->SetLocalEulerAngles(glm::vec3(glm::radians(-39.0f), glm::radians(90.0), 0));

    lightComponent = new Light(lightTrfHandle.GetComponent());
    lightHandle->AddComponent<Light>(lightComponent);
    
    Material *colMat, *floorMat , *wallMat;

#if (debugMeshForLight)
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
#endif

    floorHandle = worldObj->CreateEntity("floor");
    ComponentHandle<Transform> floorTrfHandle = floorHandle->GetComponent<Transform>();
    floorTrfHandle->SetLocalPosition(glm::vec3(0, -20, 0));
    floorTrfHandle->SetLocalScale(glm::vec3(200, 200, 1));
    floorTrfHandle->SetLocalEulerAngles(glm::vec3(glm::radians(90.0), 0, 0));

    {
        std::bitset<(unsigned int)ATTRIBUTES::NUM_ATTRIBUTES> req;
        req.set((unsigned int)ATTRIBUTES::POSITION);
        req.set((unsigned int)ATTRIBUTES::COLOR);
        req.set((unsigned int)ATTRIBUTES::NORMAL);

        PrimtiveType * prim = new PrimtiveType{ PrimtiveType::TOPOLOGY_TRIANGLE_LIST };
        MeshInfo meshInfo{};
        meshInfo.attribMaskReq = req;
        meshInfo.bufferPerAttribRequired = false;
        meshInfo.isIndexed = true; // needs to be corrected, as we are using indexed mesh but not the index buffer
        meshInfo.isPrimitiveRestartEnabled = false;
        meshInfo.primitive = prim;
        MESH_TYPE meshType = MESH_TYPE::QUAD;

        Mesh * mesh = MeshFactory::GetInstance()->CreateMesh(&meshInfo, &meshType);
        floorHandle->AddComponent<Mesh>(mesh);

        ShaderDescription shaders[2];
        shaders[0].type = ShaderType::VERTEX;
        shaders[0].shaderName = "PCN_Lighting.vert";

        shaders[1].type = ShaderType::FRAGMENT;
        shaders[1].shaderName = "ColorLighting.frag";

        floorMat = MaterialFactory::GetInstance()->CreateMaterial(shaders, 2, mesh->componentId);
        floorHandle->AddComponent<Material>(floorMat);

        floorRenderer = new MeshRenderer(mesh, floorMat, floorTrfHandle.GetComponent());
        floorHandle->AddComponent<MeshRenderer>(floorRenderer);
    }

    /*
    wallHandle = worldObj->CreateEntity("wall");
    ComponentHandle<Transform> wallTrfHandle = wallHandle->GetComponent<Transform>();
    wallTrfHandle->SetLocalPosition(glm::vec3(0, 0, -40));
    wallTrfHandle->SetLocalScale(glm::vec3(20, 20, 1));
    wallTrfHandle->SetLocalEulerAngles(glm::vec3(0, glm::radians(-180.0), 0));

    {
        std::bitset<(unsigned int)ATTRIBUTES::NUM_ATTRIBUTES> req;
        req.set((unsigned int)ATTRIBUTES::POSITION);
        req.set((unsigned int)ATTRIBUTES::COLOR);
        req.set((unsigned int)ATTRIBUTES::NORMAL);

        PrimtiveType * prim = new PrimtiveType{ PrimtiveType::TOPOLOGY_TRIANGLE_LIST };
        MeshInfo meshInfo{};
        meshInfo.attribMaskReq = req;
        meshInfo.bufferPerAttribRequired = false;
        meshInfo.isIndexed = true; // needs to be corrected, as we are using indexed mesh but not the index buffer
        meshInfo.isPrimitiveRestartEnabled = false;
        meshInfo.primitive = prim;
        MESH_TYPE meshType = MESH_TYPE::QUAD;

        Mesh * mesh = MeshFactory::GetInstance()->CreateMesh(&meshInfo, &meshType);
        wallHandle->AddComponent<Mesh>(mesh);

        ShaderDescription shaders[2];
        shaders[0].type = ShaderType::VERTEX;
        shaders[0].shaderName = "PCN_Lighting.vert";

        shaders[1].type = ShaderType::FRAGMENT;
        shaders[1].shaderName = "ColorLighting.frag";

        wallMat = MaterialFactory::GetInstance()->CreateMaterial(shaders, 2, mesh->componentId);
        wallHandle->AddComponent<Material>(wallMat);

        wallRenderer = new MeshRenderer(mesh, wallMat, wallTrfHandle.GetComponent());
        wallHandle->AddComponent<MeshRenderer>(wallRenderer);
    }
    */
}

SceneManagerScript::~SceneManagerScript()
{
    {
        ComponentHandle<Mesh> mesh = floorHandle->GetComponent<Mesh>();
        MeshFactory::GetInstance()->DestroyMesh(mesh->componentId);
        mesh.DestroyComponent();
        ComponentHandle<Material> mat = floorHandle->GetComponent<Material>();
        mat.DestroyComponent();
    }
    worldObj->DestroyEntity(floorHandle);

#if (debugMeshForLight)
    {
        ComponentHandle<Mesh> mesh = lightHandle->GetComponent<Mesh>();
        MeshFactory::GetInstance()->DestroyMesh(mesh->componentId);
        mesh.DestroyComponent();
        ComponentHandle<Material> mat = lightHandle->GetComponent<Material>();
        mat.DestroyComponent();
    }
#endif

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
