#include "VulkanInterface.h"
#include <VulkanManager.h>
#include <WindowManager.h>
#include <PresentationEngine.h>
#include <VkAttachmentFactory.h>
#include <VkRenderPassFactory.h>
#include <VkFrameBufferFactory.h>

VulkanInterface::VulkanInterface()
{
}

void VulkanInterface::Init()
{
    VulkanManager::GetInstance()->Init();
    VulkanManager::GetInstance()->CreateSurface(WindowManager::GetInstance()->glfwWindow);
    PresentationEngine::GetInstance()->Init(VulkanManager::GetInstance()->GetSurface(), VulkanManager::GetInstance()->GetSurfaceFormat());
    VkAttachmentFactory::GetInstance()->Init();
    VkRenderPassFactory::GetInstance()->Init();
    VkFrameBufferFactory::GetInstance()->Init();
}

void VulkanInterface::DeInit()
{
    VkFrameBufferFactory::GetInstance()->DeInit();
    delete VkFrameBufferFactory::GetInstance();

    VkRenderPassFactory::GetInstance()->DeInit();
    delete VkRenderPassFactory::GetInstance();

    VkAttachmentFactory::GetInstance()->DeInit();
    delete VkAttachmentFactory::GetInstance();

    PresentationEngine::GetInstance()->DeInit();
    delete PresentationEngine::GetInstance();

    VulkanManager::GetInstance()->DeInit();
    delete VulkanManager::GetInstance();
}

uint32_t VulkanInterface::FindBestDepthFormat(ImageFormat * imageFormat, uint32_t count)
{
    return VkAttachmentFactory::GetInstance()->FindBestDepthFormat(imageFormat, count);
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

void VulkanInterface::CreateRenderPass(
    const RenderPassAttachmentInfo* renderpassAttachmentList, uint32_t attachmentCount,
    const SubpassInfo* subpassList, uint32_t subpassCount,
    const SubpassDependency* dependencyList, uint32_t dependencyCount,
    uint32_t& renderPassId
    )
{
    VkRenderPassFactory::GetInstance()->CreateRenderPass(renderpassAttachmentList, attachmentCount,
        subpassList, subpassCount, dependencyList, dependencyCount, renderPassId
    );
}

void VulkanInterface::DestroyRenderPass(uint32_t id)
{
    VkRenderPassFactory::GetInstance()->DestroyRenderPass(id);
}

void VulkanInterface::CreateFrameBuffer(uint32_t numFrameBuffers, uint32_t * imageViewId, 
    uint32_t viewsPerFB, uint32_t renderPassId, uint32_t width, uint32_t height, uint32_t * ids)
{
    VkRenderPass * renderPass = VkRenderPassFactory::GetInstance()->GetRenderPass(renderPassId);

    VkImageView * viewList = new VkImageView[numFrameBuffers * viewsPerFB];
    for (uint32_t i = 0; i < numFrameBuffers * viewsPerFB; i++)
    {
        viewList[i] = *VkAttachmentFactory::GetInstance()->GetImageView(imageViewId[i]);
    }

    VkFrameBufferFactory::GetInstance()->CreateFrameBuffer(numFrameBuffers, viewList, 
        viewsPerFB, renderPass, width, height, ids);
        
    delete [] viewList;
}

void VulkanInterface::DestroyFrameBuffer(uint32_t * pid, uint32_t count)
{
    VkFrameBufferFactory::GetInstance()->DestroyFrameBuffer(pid, count);
}

VulkanInterface::~VulkanInterface()
{

}
