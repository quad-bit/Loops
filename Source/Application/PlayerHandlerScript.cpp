#include "PlayerHandlerScript.h"
#include <ECS_Setting.h>
#include <EntityHandle.h>

PlayerHandlerScript::PlayerHandlerScript()
{
    scriptName = typeid(this).raw_name();
}

void PlayerHandlerScript::Init()
{
    playerHandle = worldObj->CreateEntity();
    head = worldObj->CreateEntity();
    torso = worldObj->CreateEntity();
    leftArm = worldObj->CreateEntity();
    rightArm = worldObj->CreateEntity();
    rightLeg = worldObj->CreateEntity();
    leftLeg = worldObj->CreateEntity();
}

void PlayerHandlerScript::Update(float dt)
{
}

void PlayerHandlerScript::DeInit()
{
    
}

PlayerHandlerScript::~PlayerHandlerScript()
{
}
