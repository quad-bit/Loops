#include "EngineManager.h"

void main()
{
    EngineManager::GetInstance()->Init();

    EngineManager::GetInstance()->Update();

    EngineManager::GetInstance()->DeInit();
    delete EngineManager::GetInstance();
}