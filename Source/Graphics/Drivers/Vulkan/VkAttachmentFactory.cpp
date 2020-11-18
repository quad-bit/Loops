#include "VkAttachmentFactory.h"
#include "VulkanUtility.h"
#include "PresentationEngine.h"
#include "VulkanMemoryManager.h"
#include "VkRenderingUnwrapper.h"
#include <RenderingWrapper.h>
#include <Assertion.h>
#include <algorithm>

VkAttachmentFactory *VkAttachmentFactory::instance = nullptr;
uint32_t VkAttachmentFactory::imageId = 0;

AttachmentInfo * VkAttachmentFactory::UnwrapImageInfo(ImageInfo * imageInfo)
{
    AttachmentInfo* vulkanImageInfo = new AttachmentInfo();

    vulkanImageInfo->colorSpace = UnWrapColorSpace( imageInfo->colorSpace) ;
    vulkanImageInfo->viewType = UnWrapImageViewDegree(imageInfo->degree);
    vulkanImageInfo->imageType = UnWrapImageDegree(imageInfo->degree);
    vulkanImageInfo->format = UnWrapFormat(imageInfo->format);
    vulkanImageInfo->height = imageInfo->height;
    vulkanImageInfo->layers = imageInfo->layers;
    vulkanImageInfo->mips = imageInfo->mips;
    vulkanImageInfo->width = imageInfo->width;
    vulkanImageInfo->sampleCount = UnWrapSampleCount(imageInfo->sampleCount);
    vulkanImageInfo->usage = UnwrapUsage(imageInfo->usage);
    vulkanImageInfo->initialLayout = UnWrapImageLayout(imageInfo->initialLayout);
    return vulkanImageInfo;
}

void VkAttachmentFactory::Init()
{

}

void VkAttachmentFactory::DeInit()
{
    for (uint32_t i = 0; i < attachmentList.size(); i++)
    {
        delete attachmentList[i];
    }
    attachmentList.clear();
}

void VkAttachmentFactory::Update()
{

}

VkAttachmentFactory *VkAttachmentFactory::GetInstance()
{
    if (instance == nullptr)
    {
        instance = new VkAttachmentFactory();
    }
    return instance;
}

VkAttachmentFactory::~VkAttachmentFactory()
{
}


uint32_t VkAttachmentFactory::FindBestDepthFormat(ImageFormat * imageFormat, uint32_t count)
{
    VkFormat * formatList = new VkFormat[count];
    VkFormatProperties props = {};
    
    for (uint32_t i = 0; i < count; i++)
    {
        formatList[i] = UnWrapFormat( imageFormat[i] );
        vkGetPhysicalDeviceFormatProperties(*CoreObjects::physicalDeviceObj, formatList[i], &props);
        if (props.optimalTilingFeatures & VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT)
        {
            return i;
        }
    }

    delete [] formatList;

    return -1;
}

void VkAttachmentFactory::CreateColorAttachment(ImageInfo * info, uint32_t count, bool defaultTarget, vector<uint32_t>* ids)
{
    AttachmentInfo * attachmentInfo = UnwrapImageInfo(info);
    
    if (defaultTarget == true)
    {
        std::vector<VkImage> * imageVector = (PresentationEngine::GetInstance()->CreateSwapchainImage(attachmentInfo, count));
        std::vector<VkImageView> * imageViewVector = PresentationEngine::GetInstance()->CreateSwapchainImageViews(attachmentInfo, count);
        
        for (uint32_t i = 0; i < count; i++)
        {
            AttachmentWrapper * temp = new AttachmentWrapper;
            temp->id = GetId();
            temp->image = &imageVector->at(i);
            temp->imageView = &imageViewVector->at(i);
            temp->imageMemory = nullptr;
            temp->usage = attachmentInfo->usage;

            ids->push_back(temp->id);
            attachmentList.push_back(temp);
        }
    }

    delete attachmentInfo;
}
/*
void VkAttachmentFactory::CreateDepthAttachment(ImageInfo * info, uint32_t count, bool stencilRequired, bool defaultTarget, vector<uint32_t>* ids)
{
    AttachmentInfo * attachmentInfo = UnwrapImageInfo(info);
    //CoreObjects::bestDepthFormat = FindBestDepthFormat(attachmentInfo->format);
    //attachmentInfo->format = CoreObjects::bestDepthFormat;
    
    bool stencilAvailable = false;
    
    if(stencilRequired)
        if (attachmentInfo->format == VK_FORMAT_D32_SFLOAT_S8_UINT || attachmentInfo->format == VK_FORMAT_D24_UNORM_S8_UINT || attachmentInfo->format == VK_FORMAT_D16_UNORM_S8_UINT)
            stencilAvailable = true;

    VkImageCreateInfo createInfo{};
    createInfo.arrayLayers = 1;
    createInfo.extent.width = attachmentInfo->width;
    createInfo.extent.height = attachmentInfo->height;
    createInfo.extent.depth = 1;
    createInfo.flags = 0;
    createInfo.format = attachmentInfo->format;
    createInfo.imageType = attachmentInfo->imageType;
    createInfo.initialLayout = attachmentInfo->initialLayout;
    createInfo.mipLevels = 1;
    createInfo.pQueueFamilyIndices = nullptr;
    createInfo.queueFamilyIndexCount = 0;
    createInfo.samples = attachmentInfo->sampleCount;
    createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
    createInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
    createInfo.tiling = VK_IMAGE_TILING_OPTIMAL;
    createInfo.usage = attachmentInfo->usage;
    
    for (uint32_t i = 0; i < count; i++)
    {
        AttachmentWrapper * temp = new AttachmentWrapper;
        temp->id = GetId();
        temp->image = new VkImage();
        temp->imageView = new VkImageView();
        temp->imageMemory = new VkDeviceMemory();
        temp->usage = attachmentInfo->usage;
        ids->push_back(temp->id);

        ErrorCheck(vkCreateImage(*CoreObjects::logicalDeviceObj, &createInfo, CoreObjects::pAllocator, temp->image));

        VulkanMemoryManager::GetSingleton()->AllocateImageMemory(temp->image, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, temp->imageMemory);
        vkBindImageMemory(*CoreObjects::logicalDeviceObj, *temp->image, *temp->imageMemory, 0);

        VkImageViewCreateInfo viewCreateInfo{};
        viewCreateInfo.components = { VK_COMPONENT_SWIZZLE_IDENTITY,VK_COMPONENT_SWIZZLE_IDENTITY,VK_COMPONENT_SWIZZLE_IDENTITY,VK_COMPONENT_SWIZZLE_IDENTITY };
        viewCreateInfo.format = attachmentInfo->format;
        viewCreateInfo.image = *temp->image;
        viewCreateInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
        viewCreateInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT | (stencilAvailable ? VK_IMAGE_ASPECT_STENCIL_BIT : 0);
        viewCreateInfo.subresourceRange.baseArrayLayer = 0;
        viewCreateInfo.subresourceRange.baseMipLevel = 0;
        viewCreateInfo.subresourceRange.layerCount = 1;
        viewCreateInfo.subresourceRange.levelCount = 1;
        viewCreateInfo.viewType = attachmentInfo->viewType;

        ErrorCheck(vkCreateImageView(*CoreObjects::logicalDeviceObj, &viewCreateInfo, CoreObjects::pAllocator, temp->imageView));
        
        attachmentList.push_back(temp);
    }

    delete attachmentInfo;
}
*/

void VkAttachmentFactory::CreateDepthAttachment(VkImageCreateInfo * info, uint32_t count, 
    VkImageViewCreateInfo * viewInfo, bool stencilRequired, bool defaultTarget, vector<uint32_t>* ids)
{
    if (info == nullptr)
    {
        ASSERT_MSG(0, " Info null");
        std::exit(-1);
    }

    for (uint32_t i = 0; i < count; i++)
    {
        AttachmentWrapper * temp = new AttachmentWrapper;
        temp->id = GetId();
        temp->image = new VkImage();
        temp->imageView = new VkImageView();
        temp->imageMemory = new VkDeviceMemory();
        temp->usage = info[i].usage;
        ids->push_back(temp->id);

        ErrorCheck(vkCreateImage(*CoreObjects::logicalDeviceObj, &info[i], CoreObjects::pAllocator, temp->image));

        VulkanMemoryManager::GetSingleton()->AllocateImageMemory(temp->image, 
            VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, temp->imageMemory);
        vkBindImageMemory(*CoreObjects::logicalDeviceObj, *temp->image, *temp->imageMemory, 0);

        bool stencilAvailable = false;
        if (stencilRequired)
            if (info[i].format == VK_FORMAT_D32_SFLOAT_S8_UINT || info[i].format == VK_FORMAT_D24_UNORM_S8_UINT
                || info[i].format == VK_FORMAT_D16_UNORM_S8_UINT)
                stencilAvailable = true;

        viewInfo[i].image = *temp->image;
        viewInfo[i].subresourceRange.aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT | (stencilAvailable ? VK_IMAGE_ASPECT_STENCIL_BIT : 0);

        ErrorCheck(vkCreateImageView(*CoreObjects::logicalDeviceObj, &viewInfo[i], CoreObjects::pAllocator, temp->imageView));

        attachmentList.push_back(temp);
    }
}

void VkAttachmentFactory::DestroyAttachment(vector<uint32_t> ids, bool defaultTarget)
{
    std::reverse(std::begin(ids), std::end(ids));

    bool swapChainDestroyed = false;

    for (uint32_t i = 0; i < ids.size(); i++)
    {
        std::vector<AttachmentWrapper *>::iterator it;
        it = std::find_if(attachmentList.begin(), attachmentList.end(), [&](AttachmentWrapper * e) { return e->id == ids[i]; });

        ASSERT_MSG(it != attachmentList.end(), "Image id not found");

        if (defaultTarget == true && (*it)->usage == VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT)
        {
            if (swapChainDestroyed == false)
            {
                PresentationEngine::GetInstance()->DestroySwapChainImageView();
                PresentationEngine::GetInstance()->DestroySwapChain();
                swapChainDestroyed = true;
            }
        }
        else
            (*it)->DeActivateAttachment();
    }
}

VkImageView * VkAttachmentFactory::GetImageView(uint32_t id)
{
    std::vector<AttachmentWrapper *>::iterator it;
    it = std::find_if(attachmentList.begin(), attachmentList.end(), [&](AttachmentWrapper * e) { return e->id == id; });

    return (*it)->imageView;
}

uint32_t VkAttachmentFactory::GetId()
{
    return imageId++;
}

void AttachmentWrapper::DeActivateAttachment()
{
    if (*imageView != VK_NULL_HANDLE)
        vkDestroyImageView(*CoreObjects::logicalDeviceObj, *imageView, CoreObjects::pAllocator);
    if (*imageMemory != VK_NULL_HANDLE)
        vkFreeMemory(*CoreObjects::logicalDeviceObj, *imageMemory, CoreObjects::pAllocator);
    if (*image != VK_NULL_HANDLE)
        vkDestroyImage(*CoreObjects::logicalDeviceObj, *image, CoreObjects::pAllocator);

    if(imageView != nullptr)
        delete imageView;
    if (imageMemory != nullptr)
        delete imageMemory;
    if (image != nullptr)
        delete image;

    attachmentActive = false;
}
