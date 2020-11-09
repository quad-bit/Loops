#include "VulkanInterface.h"
#include "VulkanManager.h"
#include "WindowManager.h"
#include "PresentationEngine.h"
#include <VkAttachmentFactory.h>


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

void VulkanInterface::CreateRenderTarget(ImageInfo * info, uint32_t count, bool defaultTarget, vector<uint32_t>* ids)
{
    VkAttachmentFactory::GetInstance()->CreateColorAttachment(info, count, defaultTarget, ids);
}

VulkanInterface::~VulkanInterface()
{

}
