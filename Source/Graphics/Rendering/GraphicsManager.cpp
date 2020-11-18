#include "GraphicsManager.h"
#include "WindowManager.h"
#include <Settings.h>
#include "VulkanInterface.h"
#include "RenderingInterface.h"


GraphicsManager* GraphicsManager::instance = nullptr;

void GraphicsManager::Init(uint32_t winWidth, uint32_t winHeight, std::string winName)
{
    Settings::windowWidth   = winWidth;
    Settings::windowHeight  = winHeight;
    Settings::windowName    = winName;
    WindowManager::GetInstance()->Init();
    
#if (RENDERING_API == VULKAN)
    apiInterface = new VulkanInterface();
    renderingInterfaceObj = new RenderingInterface<VulkanInterface>();
#elif (RENDERING_API == DX)
    apiInterface = new DxInterface();
#endif
    
    renderingInterfaceObj->Init(apiInterface);
    renderingInterfaceObj->SetupRenderer();
}

void GraphicsManager::DeInit()
{
    renderingInterfaceObj->DislogeRenderer();
    renderingInterfaceObj->DeInit();
    delete renderingInterfaceObj;

    delete apiInterface;

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
