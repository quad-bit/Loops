#include "EngineManager.h"
#include <stdio.h>

void main()
{
    EngineManager::GetInstance()->Init();

    EngineManager::GetInstance()->Update();

    EngineManager::GetInstance()->DeInit();
    delete EngineManager::GetInstance();

    getchar();
}