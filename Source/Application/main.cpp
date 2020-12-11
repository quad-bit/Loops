#include "EngineManager.h"
#include <stdio.h>
#include "ApplicationController.h"

void main()
{
    ApplicationController appController;

    EngineManager::GetInstance()->Init();
    appController.Init();

    appController.Update();
    EngineManager::GetInstance()->Update();

    appController.DeInit();
    EngineManager::GetInstance()->DeInit();
    delete EngineManager::GetInstance();

    getchar();
}