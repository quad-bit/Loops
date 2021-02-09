#include "EngineManager.h"
#include <CoreManager.h>
#include <Settings.h>
#include <GraphicsManager.h>
#include <ECS_EngineManager.h>
#include <InputManager.h>
#include "SceneManager.h"
EngineManager* EngineManager::instance = nullptr;

void EngineManager::Init()
{
    //PLOGD << "ENGINE MANAGER Init";
    CoreManager::GetInstance()->Init();
    GraphicsManager::GetInstance()->Init(800, 600, "Loops");
    ECS_Manager::GetInstance()->Init();
    sceneManagerObj = new SceneManager();
}

void EngineManager::DeInit()
{
    delete sceneManagerObj;

    ECS_Manager::GetInstance()->DeInit();
    delete ECS_Manager::GetInstance();

    GraphicsManager::GetInstance()->DeInit();
    delete GraphicsManager::GetInstance();

    CoreManager::GetInstance()->DeInit();
    delete CoreManager::GetInstance();
}

void EngineManager::Update()
{
    // Before the update, will run just once
    {
        GraphicsManager::GetInstance()->PreUpdate();
    }

    while (GraphicsManager::GetInstance()->IsWindowActive())
    {
        InputManager::GetInstance()->Update();
        CoreManager::GetInstance()->Update();
        ECS_Manager::GetInstance()->Update();
        GraphicsManager::GetInstance()->Update();
    }

    // After the update, will run just once
    {
        GraphicsManager::GetInstance()->PostUpdate();
    }
}

EngineManager * EngineManager::GetInstance()
{
    if (instance == nullptr)
    {
        instance = new EngineManager();
    }
    return instance;
}

EngineManager::~EngineManager()
{
    
}
