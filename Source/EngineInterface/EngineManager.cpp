#include "EngineManager.h"
#include <CoreManager.h>
#include <Settings.h>
#include <GraphicsManager.h>
#include <ECS_EngineManager.h>
#include <InputManager.h>
#include <Timer.h>
#include <SceneManager.h>

EngineManager* EngineManager::instance = nullptr;

void EngineManager::Init()
{
    //PLOGD << "ENGINE MANAGER Init";
    Settings::maxFrameRate = 60;
    CoreManager::GetInstance()->Init();
    GraphicsManager::GetInstance()->Init(1200, 1000, "Loops");
    ECS_Manager::GetInstance()->Init();
    sceneManagerObj = new SceneManager();
    Timer::GetInstance()->Init();
}

void EngineManager::DeInit()
{
    Timer::GetInstance()->DeInit();
    delete Timer::GetInstance();

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

    typedef double precision;

    precision msPerUpdate = 1.0f / (precision)Settings::maxFrameRate;
    precision lag = 0.0f;
    while (GraphicsManager::GetInstance()->IsWindowActive())
    {
        //Timer::GetInstance()->Tick();
        lag += Timer::GetInstance()->GetDeltaTime<precision>();

        InputManager::GetInstance()->Update();
        CoreManager::GetInstance()->Update();

        uint16_t iterations = 0, maxIterations = 6;
        while (lag >= msPerUpdate && iterations < maxIterations)
        {
            ECS_Manager::GetInstance()->Update(0.0f);
            lag -= msPerUpdate;
            iterations++;
        }

        // to make it more smooth pass this(lag / MS_PER_UPDATE) to renderer, advance the render
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
