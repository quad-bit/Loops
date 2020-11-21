#include "VkRenderPassFactory.h"
#include "VkRenderingUnwrapper.h"
#include "VulkanUtility.h"
#include <Assertion.h>
#include <algorithm>
#include <array>

VkRenderPassFactory* VkRenderPassFactory::instance = nullptr;
uint32_t VkRenderPassFactory::renderpassIdCounter = 0;

uint32_t VkRenderPassFactory::GetId()
{
    return renderpassIdCounter++;
}

void VkRenderPassFactory::Init()
{
    refCounter = 0;
}

void VkRenderPassFactory::DeInit()
{
    for (uint32_t i = 0; i < renderpassList.size(); i++)
    {
        delete renderpassList[i];
    }
    renderpassList.clear();
}

void VkRenderPassFactory::Update()
{

}

VkRenderPassFactory * VkRenderPassFactory::GetInstance()
{
    if (instance == nullptr)
    {
        instance = new VkRenderPassFactory();
    }
    return instance;
}

VkRenderPassFactory::~VkRenderPassFactory()
{

}

void VkRenderPassFactory::CreateRenderPass(const VkAttachmentDescription * renderpassAttachmentList, uint32_t attachmentCount, const VkSubpassDescription * subpassList, uint32_t subpassCount, const VkSubpassDependency * dependencyList, uint32_t dependencyCount, uint32_t & renderPassId)
{
    RenderpassWrapper * info = new RenderpassWrapper();
    info->id = GetId();

    VkRenderPassCreateInfo renderPassCreateInfo{};
    renderPassCreateInfo.attachmentCount = attachmentCount;
    renderPassCreateInfo.dependencyCount = dependencyCount;
    renderPassCreateInfo.pAttachments = renderpassAttachmentList;
    renderPassCreateInfo.pDependencies = nullptr;
    renderPassCreateInfo.pSubpasses = subpassList;
    renderPassCreateInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
    renderPassCreateInfo.subpassCount = subpassCount;

    ErrorCheck(vkCreateRenderPass(*CoreObjects::logicalDeviceObj, &renderPassCreateInfo, CoreObjects::pAllocator, &info->renderPass));

    renderpassList.push_back(info);
    renderpassIdCounter = info->id;
}

void VkRenderPassFactory::CreateRenderPass(const VkAttachmentDescription * renderpassAttachmentList, 
    uint32_t attachmentCount, const VkSubpassDescription * subpassList, uint32_t 
    subpassCount, const VkSubpassDependency * dependencyList, 
    uint32_t dependencyCount, const VkRenderPassBeginInfo beginInfo, uint32_t & renderPassId)
{
    RenderpassWrapper * info = new RenderpassWrapper();
    info->id = GetId();

    VkRenderPassCreateInfo renderPassCreateInfo{};
    renderPassCreateInfo.attachmentCount = attachmentCount;
    renderPassCreateInfo.dependencyCount = dependencyCount;
    renderPassCreateInfo.pAttachments = renderpassAttachmentList;
    renderPassCreateInfo.pDependencies = nullptr;
    renderPassCreateInfo.pSubpasses = subpassList;
    renderPassCreateInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
    renderPassCreateInfo.subpassCount = subpassCount;

    ErrorCheck(vkCreateRenderPass(*CoreObjects::logicalDeviceObj, &renderPassCreateInfo, CoreObjects::pAllocator, &info->renderPass));
    info->beginInfo = beginInfo;

    renderpassList.push_back(info);
    renderpassIdCounter = info->id;

    info->clearValue[1].depthStencil.depth = Settings::depthClearValue;
    info->clearValue[1].depthStencil.stencil = (uint32_t)Settings::stencilClearValue;

    // float32 is chosen because VK_FORMAT_B8G8R8A8_UNORM is preferred to be the format UNORM is unsigned normalised which is floating point
    // the type float32 / int / uint should be selected based on the format
    info->clearValue[0].color.float32[0] = Settings::clearColorValue[0];
    info->clearValue[0].color.float32[1] = Settings::clearColorValue[1];
    info->clearValue[0].color.float32[2] = Settings::clearColorValue[2];
    info->clearValue[0].color.float32[3] = Settings::clearColorValue[3];
}

void VkRenderPassFactory::SetRenderPassBeginInfo(const VkRenderPassBeginInfo beginInfo, uint32_t renderpassId)
{
    std::vector<RenderpassWrapper*>::iterator it;
    it = std::find_if(renderpassList.begin(), renderpassList.end(), [&](RenderpassWrapper * e) { return e->id == renderpassId; });

    ASSERT_MSG(it != renderpassList.end(), "Image id not found");

    (*it)->beginInfo = beginInfo;
    (*it)->beginInfo.clearValueCount = (uint32_t)(*it)->clearValue.size();
    (*it)->beginInfo.pClearValues = (*it)->clearValue.data();
}

void VkRenderPassFactory::DestroyRenderPass(uint32_t id)
{
    std::vector<RenderpassWrapper*>::iterator it;
    it = std::find_if(renderpassList.begin(), renderpassList.end(), [&](RenderpassWrapper * e) { return e->id == id; });

    ASSERT_MSG(it != renderpassList.end(), "Image id not found");

    vkDestroyRenderPass(*CoreObjects::logicalDeviceObj, (*it)->renderPass, CoreObjects::pAllocator);
}

VkRenderPass * VkRenderPassFactory::GetRenderPass(uint32_t id)
{
    std::vector<RenderpassWrapper*>::iterator it;
    it = std::find_if(renderpassList.begin(), renderpassList.end(), [&](RenderpassWrapper * e) { return e->id == id; });

    ASSERT_MSG(it != renderpassList.end(), "Image id not found");

    return &(*it)->renderPass;
}

std::vector<VkClearValue>* VkRenderPassFactory::GetClearValue(uint32_t renderpassId)
{
    std::vector<RenderpassWrapper*>::iterator it;
    it = std::find_if(renderpassList.begin(), renderpassList.end(), [&](RenderpassWrapper * e) { return e->id == renderpassId; });

    ASSERT_MSG(it != renderpassList.end(), "Image id not found");

    return &(*it)->clearValue;
}

void VkRenderPassFactory::SetClearColor(std::vector<VkClearValue> clearValue, uint32_t renderPassId)
{
    std::vector<RenderpassWrapper*>::iterator it;
    it = std::find_if(renderpassList.begin(), renderpassList.end(), [&](RenderpassWrapper * e) { return e->id == renderPassId; });

    ASSERT_MSG(it != renderpassList.end(), "Image id not found");

    (*it)->clearValue = clearValue;

    return;

    (*it)->clearValue[1].depthStencil.depth   = clearValue[1].depthStencil.depth;
    (*it)->clearValue[1].depthStencil.stencil = clearValue[1].depthStencil.stencil;

    // float32 is chosen because VK_FORMAT_B8G8R8A8_UNORM is preferred to be the format UNORM is unsigned normalised which is floating point
    // the type float32 / int / uint should be selected based on the format
    (*it)->clearValue[0].color.float32[0] = clearValue[0].color.float32[0];
    (*it)->clearValue[0].color.float32[1] = clearValue[0].color.float32[1];
    (*it)->clearValue[0].color.float32[2] = clearValue[0].color.float32[2];
    (*it)->clearValue[0].color.float32[3] = clearValue[0].color.float32[3];
}
