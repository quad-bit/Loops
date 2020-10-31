#include "GraphicsManager.h"
#include "WindowManager.h"
#include "Settings.h"

using namespace Loops::Graphics;
using namespace Loops::Graphics::Windowing;

GraphicsManager* GraphicsManager::instance = nullptr;

void GraphicsManager::Init(uint32_t winWidth, uint32_t winHeight, std::string winName)
{
    Settings::windowWidth = winWidth;
    Settings::windowHeight = winHeight;
    Settings::windowName = winName;
    WindowManager::GetInstance()->Init();
}

void GraphicsManager::DeInit()
{
    WindowManager::GetInstance()->DeInit();
    delete WindowManager::GetInstance();
}

void GraphicsManager::Update()
{
    while (WindowManager::GetInstance()->Update())
    {

    }
}

GraphicsManager * GraphicsManager::GetInstance()
{
    if (instance == nullptr)
    {
        instance = new GraphicsManager();
    }
    return instance;
}

GraphicsManager::~GraphicsManager()
{
}
