#include "CorePrecompiled.h"
#include "PlayerHandlerScript.h"
#include <ECS_Setting.h>
#include <EntityHandle.h>
#include <typeinfo>
#include <Transform.h>
#include <Mesh.h>
#include <AttributeHelper.h>
#include <MeshFactory.h>
#include <BitArray.h>
#include <MaterialFactory.h>
#include <Camera.h>
#include <MeshRenderer.h>
#include <Timer.h>
#include <glm\glm.hpp>
#include <MathUtil.h>
#include "CameraController.h"

PlayerHandlerScript::PlayerHandlerScript() : Scriptable(false)
{
    scriptName = typeid(this).raw_name();

    camHandle0 = worldObj->CreateEntity();
    camHandle0->GetEntity()->entityName = "MainCamera";
    camHandle0->GetTransform()->SetLocalPosition( glm::vec3(0, 0, 10));
    camHandle0->GetTransform()->SetLocalEulerAngles(glm::vec3(0, 0, 0));

    Camera * camera = new Camera(camHandle0->GetTransform());
    camHandle0->AddComponent<Camera>(camera);

    ComponentHandle<Camera> test = camHandle0->GetComponent<Camera>();

    cameraController = new CameraController();
    camHandle0->AddComponent<Scriptable>(cameraController);


    //camHandle1 = worldObj->CreateEntity();
    //camHandle1->GetEntity()->entityName = "SecondCamera";
    //Camera * cam = new Camera(&camHandle1->GetTransform()->globalPosition);
    //camHandle1->AddComponent<Camera>(cam);

    playerHandle = worldObj->CreateEntity();
    playerHandle->GetEntity()->entityName = "player";
    Transform * playerTrf = playerHandle->GetTransform();
    playerTrf->SetLocalPosition(glm ::vec3(0, 0, 0));
    
    // TORSO
    torso = worldObj->CreateEntity();
    torso->GetEntity()->entityName = "torso";
    Transform * torsoTrf = torso->GetTransform();
    torsoTrf->SetLocalPosition(glm::vec3(0, 0, 0));

    Material * colMat;
    torsoTrf->SetParent(playerTrf);
    {
        BitArray req(10);
        req.SetBit((unsigned int)ATTRIBUTES::POSITION);
        req.SetBit((unsigned int)ATTRIBUTES::COLOR);

        PrimtiveType * prim = new PrimtiveType{ PrimtiveType::TOPOLOGY_TRIANGLE_LIST};
        MeshInfo meshInfo{};
        meshInfo.attribMaskReq = req;
        meshInfo.bufferPerAttribRequired = false;
        meshInfo.isIndexed = true;
        meshInfo.isPrimitiveRestartEnabled = false;
        meshInfo.primitive = prim;
        MESH_TYPE meshType = MESH_TYPE::CUBE;

        Mesh * torsoMesh = MeshFactory::GetInstance()->CreateMesh(&meshInfo, &meshType);
        torso->AddComponent<Mesh>(torsoMesh);

        ShaderDescription shaders[2];
        shaders[0].type = ShaderType::VERTEX;
        shaders[0].shaderName = "PC.vert";

        shaders[1].type = ShaderType::FRAGMENT;
        shaders[1].shaderName = "Color.frag";

        colMat = MaterialFactory::GetInstance()->CreateMaterial(shaders, 2, torsoMesh->componentId);
        torso->AddComponent<Material>(colMat);

        torsoMeshRenderer = new MeshRenderer(torsoMesh, colMat, torsoTrf);
        torso->AddComponent<MeshRenderer>(torsoMeshRenderer);
    }

    // HEAD
    head = worldObj->CreateEntity();
    head->GetEntity()->entityName = "head";
    Transform * headTrf = head->GetTransform();
    headTrf->SetParent(torsoTrf);
    headTrf->SetLocalPosition( glm::vec3(0, 3, 0));

    {
        BitArray req(10);
        req.SetBit((unsigned int)ATTRIBUTES::POSITION);
        req.SetBit((unsigned int)ATTRIBUTES::COLOR);

        PrimtiveType * prim = new PrimtiveType{ PrimtiveType::TOPOLOGY_TRIANGLE_LIST }; 
        MeshInfo meshInfo{};
        meshInfo.attribMaskReq = req;
        meshInfo.bufferPerAttribRequired = false;
        meshInfo.isIndexed = true;
        meshInfo.isPrimitiveRestartEnabled = false;
        meshInfo.primitive = prim;
        MESH_TYPE meshType = MESH_TYPE::CUBE;

        Mesh * headMesh = MeshFactory::GetInstance()->CreateMesh(&meshInfo, &meshType);
        head->AddComponent<Mesh>(headMesh);

        head->AddComponent<Material>(colMat);
        MaterialFactory::GetInstance()->AddMeshIds(colMat, headMesh->componentId);

        headMeshRenderer = new MeshRenderer(headMesh, colMat, headTrf);
        head->AddComponent<MeshRenderer>(headMeshRenderer);
    }

    //LEFT ARM
    leftArm = worldObj->CreateEntity();
    leftArm->GetEntity()->entityName = "leftArm";
    leftArm->GetTransform()->SetParent(torsoTrf);

    //RIGHT ARM
    rightArm = worldObj->CreateEntity();
    rightArm->GetEntity()->entityName = "rightArm";
    rightArm->GetTransform()->SetParent(torsoTrf);

    //RIGHT LEG
    rightLeg = worldObj->CreateEntity();
    rightLeg->GetEntity()->entityName = "rightLeg";
    rightLeg->GetTransform()->SetParent(torsoTrf);

    //LEFT LEG
    leftLeg = worldObj->CreateEntity();
    leftLeg->GetEntity()->entityName = "leftLeg";
    leftLeg->GetTransform()->SetParent(torsoTrf);

}

void PlayerHandlerScript::Init()
{
    
}

static uint32_t counter = 0;
void PlayerHandlerScript::Update(float dt)
{
    // head rotation
    {
        Transform * headTrf = head->GetTransform();
        
        currentAngle = glm::sin(counter++ / 100.0f);

        float angle = MathUtil::lerp(prevAngle, currentAngle, dt);
        prevAngle = currentAngle;

        headTrf->SetLocalEulerAngles(glm::vec3(0, angle, 0));
    }
}

// not getting used 
void PlayerHandlerScript::Render(float dt)
{
    //// head rotation
    //{
    //    Transform * headTrf = head->GetTransform();

    //    float angle = glm::sin(counter++ / 100.0f) * dt;
    //    PLOGD << "Render " <<dt;

    //    headTrf->SetLocalEulerAngles(glm::vec3(0, angle, 0));
    //}

}

void PlayerHandlerScript::DeInit()
{
    int k = 4;
}

PlayerHandlerScript::~PlayerHandlerScript()
{
    worldObj->DestroyEntity(playerHandle);
    {
        ComponentHandle<Mesh> torsoMesh = torso->GetComponent<Mesh>();
        MeshFactory::GetInstance()->DestroyMesh(torsoMesh->componentId);
        torsoMesh.DestroyComponent();
        //torso->RemoveComponent<Mesh>(torsoMesh.GetComponent());
        //delete torsoMesh.GetComponent(); //TODO  fix this get component function
        ComponentHandle<Material> torsoMat = torso->GetComponent<Material>();
        torsoMat.DestroyComponent();
        worldObj->DestroyEntity(torso);
    }

#if 1
    ComponentHandle<Mesh> headMesh = head->GetComponent<Mesh>();
    MeshFactory::GetInstance()->DestroyMesh(headMesh->componentId);
    headMesh.DestroyComponent();
    //head->RemoveComponent<Mesh>(headMesh.GetComponent());
    ComponentHandle<Material> headMat = head->GetComponent<Material>();
    headMat.DestroyComponent();
#endif
    worldObj->DestroyEntity(head);

    worldObj->DestroyEntity(leftArm);
    worldObj->DestroyEntity(rightArm);
    worldObj->DestroyEntity(leftLeg);
    worldObj->DestroyEntity(rightLeg);
    
    delete cameraController;
    worldObj->DestroyEntity(camHandle0);
}
