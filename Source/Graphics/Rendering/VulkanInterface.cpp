#include "VulkanInterface.h"
#include <VulkanManager.h>
#include <WindowManager.h>
#include <PresentationEngine.h>
#include <VkAttachmentFactory.h>


VulkanInterface::VulkanInterface()
{
}

void VulkanInterface::Init()
{
    VulkanManager::GetInstance()->Init();
    VulkanManager::GetInstance()->CreateSurface(WindowManager::GetInstance()->glfwWindow);
    PresentationEngine::GetInstance()->Init(VulkanManager::GetInstance()->GetSurface(), VulkanManager::GetInstance()->GetSurfaceFormat());
    VkAttachmentFactory::GetInstance()->Init();
}

void VulkanInterface::DeInit()
{
    VkAttachmentFactory::GetInstance()->DeInit();
    delete VkAttachmentFactory::GetInstance();

    PresentationEngine::GetInstance()->DeInit();
    delete PresentationEngine::GetInstance();

    VulkanManager::GetInstance()->DeInit();
    delete VulkanManager::GetInstance();
}

void VulkanInterface::CreateRenderTarget(ImageInfo * info, uint32_t count, bool defaultTarget, vector<uint32_t>* ids)
{
    VkAttachmentFactory::GetInstance()->CreateColorAttachment(info, count, defaultTarget, ids);
}

void VulkanInterface::DestroyRenderTarget(std::vector<uint32_t>* ids, bool defaultTarget)
{
    VkAttachmentFactory::GetInstance()->DestroyAttachment(*ids, defaultTarget);
}

void VulkanInterface::CreateDepthTarget(ImageInfo * info, uint32_t count, bool stencilRequired, bool defaultTarget, std::vector<uint32_t>* ids)
{
    VkAttachmentFactory::GetInstance()->CreateDepthAttachment(info, count, stencilRequired, defaultTarget, ids);
}

void VulkanInterface::DestroyDepthTarget(std::vector<uint32_t>* ids, bool defaultTarget)
{
    VkAttachmentFactory::GetInstance()->DestroyAttachment(*ids, defaultTarget);
}

VulkanInterface::~VulkanInterface()
{

}
