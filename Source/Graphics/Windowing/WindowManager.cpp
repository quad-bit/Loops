#include "WindowManager.h"

WindowManager* WindowManager::windowManagerInstance = nullptr;

WindowManager::~WindowManager()
{
}

WindowManager * WindowManager::GetInstance()
{
    if (windowManagerInstance == nullptr)
    {
        windowManagerInstance = new WindowManager();
    }
    return windowManagerInstance;
}

void WindowManager::Init()
{
    InitOSWindow();
}

void WindowManager::DeInit()
{
    DeInitOSWindow();
}

void WindowManager::Close()
{
    windowShouldRun = false;
}

bool WindowManager::Update()
{
    UpdateOSWindow();
    return windowShouldRun;
}
