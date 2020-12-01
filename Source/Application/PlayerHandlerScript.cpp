#include "PlayerHandlerScript.h"
#include <ECS_Setting.h>
#include <EntityHandle.h>
#include <typeinfo>

PlayerHandlerScript::PlayerHandlerScript()
{
    scriptName = typeid(this).raw_name();

    playerHandle = worldObj->CreateEntity();
    head = worldObj->CreateEntity();
    torso = worldObj->CreateEntity();
    leftArm = worldObj->CreateEntity();
    rightArm = worldObj->CreateEntity();
    rightLeg = worldObj->CreateEntity();
    leftLeg = worldObj->CreateEntity();
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
    worldObj->DestroyEntity(head);
    worldObj->DestroyEntity(torso);
    worldObj->DestroyEntity(leftArm);
    worldObj->DestroyEntity(rightArm);
    worldObj->DestroyEntity(leftLeg);
    worldObj->DestroyEntity(rightLeg);
}
