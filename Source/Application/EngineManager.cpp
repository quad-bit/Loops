#include "EngineManager.h"
#include <CoreManager.h>
#include <Settings.h>
#include <GraphicsManager.h>

using namespace Loops::Application;
using namespace Loops::Core;
using namespace Loops::Graphics;

EngineManager* EngineManager::instance = nullptr;

void EngineManager::Init()
{
    CoreManager::GetInstance()->Init();
    CoreManager::GetInstance()->test = 100;
    GraphicsManager::GetInstance()->Init(800, 600, "Loops");
}

void EngineManager::DeInit()
{
    GraphicsManager::GetInstance()->DeInit();
    delete GraphicsManager::GetInstance();

    CoreManager::GetInstance()->DeInit();
    delete CoreManager::GetInstance();
}

void EngineManager::Update()
{
    CoreManager::GetInstance()->Update();
    GraphicsManager::GetInstance()->Update();
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
