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

PlayerHandlerScript::PlayerHandlerScript()
{
    scriptName = typeid(this).raw_name();

    camHandle0 = worldObj->CreateEntity();
    camHandle0->GetEntity()->entityName = "MainCamera";
    Camera * camera = new Camera(&camHandle0->GetTransform()->globalPosition);
    camHandle0->AddComponent<Camera>(camera);

    camHandle1 = worldObj->CreateEntity();
    camHandle1->GetEntity()->entityName = "SecondCamera";
    Camera * cam = new Camera(&camHandle1->GetTransform()->globalPosition);
    camHandle1->AddComponent<Camera>(cam);

    playerHandle = worldObj->CreateEntity();
    playerHandle->GetEntity()->entityName = "player";
    Transform * playerTrf = playerHandle->GetTransform();
    
    // TORSO
    torso = worldObj->CreateEntity();
    torso->GetEntity()->entityName = "torso";
    Transform * torsoTrf = torso->GetTransform();
    torsoTrf->SetParent(playerTrf);
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

        Mesh * torsoMesh = MeshFactory::GetInstance()->CreateMesh(&meshInfo, &meshType);
        torso->AddComponent<Mesh>(torsoMesh);

        ShaderDescription shaders[2];
        shaders[0].type = ShaderType::VERTEX;
        shaders[0].shaderName = "PC.vert";

        shaders[1].type = ShaderType::FRAGMENT;
        shaders[1].shaderName = "Color.frag";

        //ShaderFactory::GetInstance()->CreateShader(torsoMesh->meshId, shaders, 2);
        Material * mat = MaterialFactory::GetInstance()->CreateMaterial(shaders, 2, torsoMesh->componentId);
        torso->AddComponent<Material>(mat);
    }

    // HEAD
    head = worldObj->CreateEntity();
    head->GetEntity()->entityName = "head";
    Transform * headTrf = head->GetTransform();
    headTrf->SetParent(torsoTrf);

    //TESTING
    {
        BitArray req(10);
        req.SetBit((unsigned int)ATTRIBUTES::POSITION);
        req.SetBit((unsigned int)ATTRIBUTES::COLOR);

        PrimtiveType * prim = new PrimtiveType{ PrimtiveType::TOPOLOGY_TRIANGLE_STRIP }; // TODO : Correct the topo
        MeshInfo meshInfo{};
        meshInfo.attribMaskReq = req;
        meshInfo.bufferPerAttribRequired = false;
        meshInfo.isIndexed = true;
        meshInfo.isPrimitiveRestartEnabled = false;
        meshInfo.primitive = prim;
        MESH_TYPE meshType = MESH_TYPE::CUBE;

        Mesh * headMesh = MeshFactory::GetInstance()->CreateMesh(&meshInfo, &meshType);
        head->AddComponent<Mesh>(headMesh);

        ShaderDescription shaders[2];
        shaders[0].type = ShaderType::VERTEX;
        shaders[0].shaderName = "PC.vert";

        shaders[1].type = ShaderType::FRAGMENT;
        shaders[1].shaderName = "Color.frag";

        //ShaderFactory::GetInstance()->CreateShader(headMesh->meshId, shaders, 2);
        Material * mat = MaterialFactory::GetInstance()->CreateMaterial(shaders, 2, headMesh->componentId);
        head->AddComponent<Material>(mat);
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
    int k = 4;
}

void PlayerHandlerScript::Update(float dt)
{
}

void PlayerHandlerScript::DeInit()
{
    int k = 4;
}

PlayerHandlerScript::~PlayerHandlerScript()
{
    worldObj->DestroyEntity(playerHandle);

    ComponentHandle<Mesh> torsoMesh = torso->GetComponent<Mesh>();
    MeshFactory::GetInstance()->DestroyMesh(torsoMesh->componentId);
    torsoMesh.DestroyComponent();
    //torso->RemoveComponent<Mesh>(torsoMesh.GetComponent());
    //delete torsoMesh.GetComponent(); //TODO  fix this get component function
    ComponentHandle<Material> torsoMat = torso->GetComponent<Material>();
    torsoMat.DestroyComponent();

    worldObj->DestroyEntity(torso);

    ComponentHandle<Mesh> headMesh = head->GetComponent<Mesh>();
    MeshFactory::GetInstance()->DestroyMesh(headMesh->componentId);
    headMesh.DestroyComponent();
    //head->RemoveComponent<Mesh>(headMesh.GetComponent());
    ComponentHandle<Material> headMat = head->GetComponent<Material>();
    headMat.DestroyComponent();
    worldObj->DestroyEntity(head);

    worldObj->DestroyEntity(leftArm);
    worldObj->DestroyEntity(rightArm);
    worldObj->DestroyEntity(leftLeg);
    worldObj->DestroyEntity(rightLeg);
    worldObj->DestroyEntity(camHandle0);
}
