#include "VulkanInterface.h"
#include <VulkanManager.h>
#include <WindowManager.h>
#include <PresentationEngine.h>
#include <VkAttachmentFactory.h>
#include <VkRenderPassFactory.h>
#include <VkFrameBufferFactory.h>
#include <VkRenderingUnwrapper.h>
#include <VkCommandBufferFactory.h>

VkAttachmentDescription * VulkanInterface::UnwrapAttachmentDesc(const RenderPassAttachmentInfo * renderpassAttachmentList, uint32_t attachmentCount)
{
    VkAttachmentDescription* attachmentDescriptions = new VkAttachmentDescription[attachmentCount];

    for (uint32_t i = 0; i < attachmentCount; i++)
    {
        const RenderPassAttachmentInfo* obj = renderpassAttachmentList + i;
        attachmentDescriptions[i].initialLayout = UnWrapImageLayout(obj->initialLayout);
        attachmentDescriptions[i].finalLayout = UnWrapImageLayout(obj->finalLayout);
        attachmentDescriptions[i].format = UnWrapFormat(obj->format);
        attachmentDescriptions[i].loadOp = UnWrapLoadOp(obj->loadOp);
        attachmentDescriptions[i].storeOp = UnWrapStoreOp(obj->storeOp);
        attachmentDescriptions[i].stencilLoadOp = UnWrapLoadOp(obj->stencilLoadOp);
        attachmentDescriptions[i].stencilStoreOp = UnWrapStoreOp(obj->stencilLStoreOp);
        attachmentDescriptions[i].samples = UnWrapSampleCount(obj->sampleCount);
        attachmentDescriptions[i].flags = 0;
    }

    return attachmentDescriptions;
}

VkSubpassDescription * VulkanInterface::UnwrapSubpassDesc(const SubpassInfo * subpassList, uint32_t subpassCount)
{
    VkSubpassDescription * subpassDescriptions = new VkSubpassDescription[subpassCount];

    //for each (SubpassInfo* obj in subpassList)
    for (uint32_t i = 0; i < subpassCount; i++)
    {
        const SubpassInfo* obj = subpassList + i;

        //Color
        if (obj->colorAttachmentCount > 0)
        {
            subpassDescriptions[i].colorAttachmentCount = obj->colorAttachmentCount;
            VkAttachmentReference* refCol = new VkAttachmentReference[obj->colorAttachmentCount];
            for (uint32_t j = 0; j < subpassDescriptions[i].colorAttachmentCount; j++)
            {
                AttachmentRef * aref = obj->pColorAttachments + j;
                refCol[i] = UnWrapAttachmentRef(*aref);
                //refs.push_back(ref);
            }
            subpassDescriptions[i].pColorAttachments = refCol;
            //refCounter += obj->colorAttachmentCount;
        }
        else
        {
            subpassDescriptions[i].pColorAttachments = nullptr;
            subpassDescriptions[i].colorAttachmentCount = 0;
        }

        //Input
        if (obj->inputAttachmentCount > 0)
        {
            subpassDescriptions[i].inputAttachmentCount = obj->inputAttachmentCount;
            VkAttachmentReference* refInp = new VkAttachmentReference[obj->inputAttachmentCount];

            for (uint32_t j = 0; j < subpassDescriptions[i].inputAttachmentCount; j++)
            {
                AttachmentRef * aref = obj->pInputAttachments + j;
                refInp[i] = UnWrapAttachmentRef(*aref);
            }
            subpassDescriptions[i].pInputAttachments = refInp;
            //refCounter += obj->inputAttachmentCount;
        }
        else
        {
            subpassDescriptions[i].pInputAttachments = nullptr;
            subpassDescriptions[i].inputAttachmentCount = 0;
        }

        //Depth
        if (obj->pDepthStencilAttachment != nullptr)
        {
            VkAttachmentReference *refDepth = new VkAttachmentReference;
            *refDepth = UnWrapAttachmentRef(*obj->pDepthStencilAttachment);

            subpassDescriptions[i].pDepthStencilAttachment = refDepth;
        }
        else
        {
            subpassDescriptions[i].pDepthStencilAttachment = nullptr;
        }
        //resolve, the count is missing.. might require another vector
        subpassDescriptions[i].pResolveAttachments = nullptr;
        subpassDescriptions[i].preserveAttachmentCount = 0;
        subpassDescriptions[i].pPreserveAttachments = nullptr;

        subpassDescriptions[i].pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
        subpassDescriptions[i].flags = 0;
    }

    return subpassDescriptions;
}

VkSubpassDependency * VulkanInterface::UnwrapSubpassDependency(const SubpassDependency * dependencyList, uint32_t dependencyCount)
{
    return nullptr;
}

VkImageCreateInfo VulkanInterface::UnwrapImageInfo(ImageInfo * info)
{
    VkImageCreateInfo createInfo = {};// new VkImageCreateInfo();
    
    createInfo.arrayLayers = 1;
    createInfo.extent.width = info->width;
    createInfo.extent.height = info->height;
    createInfo.extent.depth = 1;
    createInfo.flags = 0;
    createInfo.format = UnWrapFormat(info->format);
    createInfo.imageType = UnWrapImageDegree(info->degree);
    createInfo.initialLayout = UnWrapImageLayout( info->initialLayout);
    createInfo.mipLevels = 1;
    createInfo.pQueueFamilyIndices = nullptr;
    createInfo.queueFamilyIndexCount = 0;
    createInfo.samples = UnWrapSampleCount(info->sampleCount);
    createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
    createInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
    createInfo.tiling = VK_IMAGE_TILING_OPTIMAL;
    createInfo.usage = UnwrapUsage(info->usage);

    return createInfo;
}

VkImageViewCreateInfo VulkanInterface::UnwrapImageViewInfo(ImageInfo * info)
{
    VkImageViewCreateInfo viewCreateInfo = {};
    viewCreateInfo.components = { VK_COMPONENT_SWIZZLE_IDENTITY,VK_COMPONENT_SWIZZLE_IDENTITY,VK_COMPONENT_SWIZZLE_IDENTITY,VK_COMPONENT_SWIZZLE_IDENTITY };
    viewCreateInfo.format = UnWrapFormat(info->format);
    viewCreateInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
    viewCreateInfo.subresourceRange.baseArrayLayer = 0;
    viewCreateInfo.subresourceRange.baseMipLevel = 0;
    viewCreateInfo.subresourceRange.layerCount = 1;
    viewCreateInfo.subresourceRange.levelCount = 1;
    viewCreateInfo.viewType = UnWrapImageViewDegree( info->degree);

    return viewCreateInfo;
}

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

void VulkanInterface::CreateDepthTarget(ImageInfo * info, uint32_t count, bool stencilRequired, 
    bool defaultTarget, std::vector<uint32_t>* ids)
{
    VkImageCreateInfo * imageCreateInfo = new VkImageCreateInfo[count];
    VkImageViewCreateInfo * viewCreateInfo = new VkImageViewCreateInfo[count];

    for (uint32_t i = 0; i < count; i++)
    {
        imageCreateInfo[i] = UnwrapImageInfo(info);
        viewCreateInfo[i]  = UnwrapImageViewInfo(info);
    }

    VkAttachmentFactory::GetInstance()->CreateDepthAttachment(imageCreateInfo, count, viewCreateInfo,
        stencilRequired, defaultTarget, ids);

    delete[] imageCreateInfo;
    delete[] viewCreateInfo;
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
    /*VkRenderPassFactory::GetInstance()->CreateRenderPass(renderpassAttachmentList, attachmentCount,
        subpassList, subpassCount, dependencyList, dependencyCount, renderPassId
    );*/

    VkAttachmentDescription* attachmentDescList = UnwrapAttachmentDesc(renderpassAttachmentList, attachmentCount);
    VkSubpassDescription* subpassDescList = UnwrapSubpassDesc(subpassList, subpassCount);
    VkSubpassDependency* subpassDependencyList = UnwrapSubpassDependency(dependencyList, dependencyCount);

    VkRenderPassFactory::GetInstance()->CreateRenderPass(attachmentDescList, attachmentCount,
        subpassDescList, subpassCount, subpassDependencyList, dependencyCount, renderPassId);

    if (attachmentDescList != nullptr)
        delete[] attachmentDescList;

    if (subpassDescList->pColorAttachments != nullptr)
        delete[] subpassDescList->pColorAttachments;

    if (subpassDescList->pDepthStencilAttachment != nullptr)
        delete subpassDescList->pDepthStencilAttachment;

    if (subpassDescList->pInputAttachments != nullptr)
        delete[] subpassDescList->pInputAttachments;

    if (subpassDescList->pResolveAttachments != nullptr)
        delete[] subpassDescList->pResolveAttachments;

    if (subpassDescList->pPreserveAttachments != nullptr)
        delete[] subpassDescList->pPreserveAttachments;

    if (subpassDescList != nullptr)
        delete[] subpassDescList;
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

void VulkanInterface::CreateCommandBuffer(const uint32_t & poolId, uint32_t & cmdBufferId, 
    CommandBufferLevel* commandBufferLevel)
{
    VkCommandBufferLevel level;
    switch (*commandBufferLevel)
    {
    case CommandBufferLevel::PRIMARY:
        level = VkCommandBufferLevel::VK_COMMAND_BUFFER_LEVEL_PRIMARY;
        break;

    case CommandBufferLevel::SECONDARY:
        level = VkCommandBufferLevel::VK_COMMAND_BUFFER_LEVEL_SECONDARY;
        break;

    default:
        ASSERT_MSG(0, "Wrong command level");
    }

    VkCommandBufferFactory::GetInstance()->CreateCommandBuffer(poolId, level, 1, &cmdBufferId);
}

void VulkanInterface::DestroyCommandBuffer(uint32_t id)
{
    VkCommandBufferFactory::GetInstance()->DestroyCommandBuffer(id);
}

uint32_t VulkanInterface::CreateCommandPool(PipelineType * pipelineType, CommandPoolProperty * prop)
{
    uint32_t poolId = 0;

    VkQueueFlagBits queueFlag;
    switch (*pipelineType)
    {
    case PipelineType::COMPUTE :
        queueFlag = VkQueueFlagBits::VK_QUEUE_COMPUTE_BIT;
        break;

    case PipelineType::GRAPHICS:
        queueFlag = VkQueueFlagBits::VK_QUEUE_GRAPHICS_BIT;
        break;

    case PipelineType::TRANSFER:
        queueFlag = VkQueueFlagBits::VK_QUEUE_TRANSFER_BIT;
        break;

    default:
        ASSERT_MSG(0, "Queue/pipeline type not found");
    }

    VkCommandPoolCreateFlagBits poolCreateFlag;
    switch (*prop)
    {
    case CommandPoolProperty::RESET_COMMAND_BUFFER_BIT:
        poolCreateFlag = VkCommandPoolCreateFlagBits::VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT ;
        break;

    case CommandPoolProperty::TRANSIENT_BIT:
        poolCreateFlag = VkCommandPoolCreateFlagBits::VK_COMMAND_POOL_CREATE_TRANSIENT_BIT;
        break;

    case CommandPoolProperty::TRANS_RESET:
        poolCreateFlag = (VkCommandPoolCreateFlagBits)(VkCommandPoolCreateFlagBits::VK_COMMAND_POOL_CREATE_TRANSIENT_BIT |
            VkCommandPoolCreateFlagBits::VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT);
        break;

    default:
        ASSERT_MSG(0, "Queue/pipeline type not found");
    }

    VkCommandBufferFactory::GetInstance()->CreateCommandPool(poolCreateFlag, queueFlag, poolId);

    return poolId;
}

void VulkanInterface::DestroyCommandPool(uint32_t poolId)
{
    VkCommandBufferFactory::GetInstance()->DestroyCommandPool(poolId);
}

VulkanInterface::~VulkanInterface()
{

}
