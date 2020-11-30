#include "ECS_EngineManager.h"
#include "World.h"
#include "Transform.h"
#include "TransformSystem.h"
#include "Scriptable.h"
#include "ScriptableSystem.h"
#include "ECS_Setting.h"

ECS_Manager* ECS_Manager::instance = nullptr;
World * worldObj;

void ECS_Manager::Init()
{
    worldObj = new World();
    
    scriptableManager = worldObj->CreateManager<Scriptable>();
    transformManager = worldObj->CreateManager<Transform>();
    
    scriptableSystemObj = new ScriptableSystem();
    worldObj->AddSystem(scriptableSystemObj);

    transformSystemObj = new TransformSystem();
    worldObj->AddSystem(transformSystemObj);

    worldObj->Init();
}

void ECS_Manager::DeInit()
{
    worldObj->DeInit();

    //delete transformManager;
    //delete scriptableManager;
    delete worldObj;
}

void ECS_Manager::Update()
{
    worldObj->Update(0.0f);
    worldObj->Render();
}

ECS_Manager * ECS_Manager::GetInstance()
{
    if (instance == nullptr)
    {
        instance = new ECS_Manager();
    }
    return instance;
}

ECS_Manager::~ECS_Manager()
{
}
