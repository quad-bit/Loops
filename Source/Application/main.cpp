#include "EngineManager.h"

using namespace Loops::Application;

void main()
{
    EngineManager::GetInstance()->Init();

    EngineManager::GetInstance()->Update();

    EngineManager::GetInstance()->DeInit();
    delete EngineManager::GetInstance();
}