#include "PlayerHandlerScript.h"
#include <ECS_Setting.h>
#include <EntityHandle.h>
#include <typeinfo>
#include <Transform.h>
#include <Mesh.h>
#include <AttributeHelper.h>
#include <MeshFactory.h>
#include <BitArray.h>
#include <ShaderFactory.h>
#include <Shader.h>

PlayerHandlerScript::PlayerHandlerScript()
{
    scriptName = typeid(this).raw_name();

    playerHandle = worldObj->CreateEntity();
    playerHandle->GetEntity()->entityName = "player";
    Transform * playerTrf = playerHandle->GetTransform();
    
    // TORSO
    torso = worldObj->CreateEntity();
    torso->GetEntity()->entityName = "torso";
    Transform * torsoTrf = torso->GetTransform();
    torsoTrf->SetParent(playerTrf);
    
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

    ShaderType * types = new ShaderType[2];
    types[0] = ShaderType::VERTEX;
    types[1] = ShaderType::FRAGMENT;

    Shader * shaders = new Shader[2];
    shaders[0].shaderType = types[0];
    shaders[0].shaderName = "PC.vert";

    shaders[1].shaderType = types[1];
    shaders[1].shaderName = "Color.frag";

    ShaderFactory::GetInstance()->CreateShader(torsoMesh->meshId, shaders, 2);

    // HEAD
    head = worldObj->CreateEntity();
    head->GetEntity()->entityName = "head";
    Transform * headTrf = head->GetTransform();
    headTrf->SetParent(torsoTrf);

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
    //return;

    worldObj->DestroyEntity(playerHandle);
    worldObj->DestroyEntity(head);

    ComponentHandle<Mesh> torsoMesh = torso->GetComponent<Mesh>();
    MeshFactory::GetInstance()->DestroyMesh(torsoMesh->meshId);
    torsoMesh.DestroyComponent();
    torso->RemoveComponent<Mesh>(torsoMesh.GetComponent());
    //delete torsoMesh.GetComponent(); //TODO  fix this get component function

    worldObj->DestroyEntity(torso);
    worldObj->DestroyEntity(leftArm);
    worldObj->DestroyEntity(rightArm);
    worldObj->DestroyEntity(leftLeg);
    worldObj->DestroyEntity(rightLeg);
}
