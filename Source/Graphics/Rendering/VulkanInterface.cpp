#include "VulkanInterface.h"
#include "VulkanManager.h"
#include "WindowManager.h"
#include "PresentationEngine.h"

using namespace Loops::Graphics::Rendering;
using namespace Loops::Graphics::Vulkan;
using namespace Loops::Graphics::Windowing;

VulkanInterface::VulkanInterface()
{
}

void VulkanInterface::Init()
{
    VulkanManager::GetInstance()->Init();
    VulkanManager::GetInstance()->CreateSurface(WindowManager::GetInstance()->glfwWindow);
    PresentationEngine::GetInstance()->Init(VulkanManager::GetInstance()->GetSurface(), VulkanManager::GetInstance()->GetSurfaceFormat());
}

void VulkanInterface::DeInit()
{
    PresentationEngine::GetInstance()->DeInit();
    VulkanManager::GetInstance()->DeInit();
    delete VulkanManager::GetInstance();
}

VulkanInterface::~VulkanInterface()
{
}
