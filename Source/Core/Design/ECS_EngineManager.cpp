#include "ECS_EngineManager.h"
#include "World.h"
#include "Transform.h"
#include "TransformSystem.h"
#include "Scriptable.h"
#include "ScriptableSystem.h"
#include "ECS_Setting.h"
#include "EventBus.h"
#include "Mesh.h"

ECS_Manager* ECS_Manager::instance = nullptr;
World * worldObj;

void ECS_Manager::Init()
{
    worldObj = new World();
    
    scriptableManager = worldObj->CreateManager<Scriptable>();
    transformManager = worldObj->CreateManager<Transform>();
    meshManager = worldObj->CreateManager<Mesh>();
    
    scriptableSystemObj = new ScriptableSystem();
    worldObj->AddSystem(scriptableSystemObj);

    transformSystemObj = new TransformSystem();
    worldObj->AddSystem(transformSystemObj);

    worldObj->Init();
}

void ECS_Manager::DeInit()
{
    EventBus::GetInstance()->DeInit();
    delete EventBus::GetInstance();

    worldObj->DeInit();

    //delete transformManager;
    //delete scriptableManager;
    delete worldObj;

    delete transformSystemObj;
    delete scriptableSystemObj;

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
