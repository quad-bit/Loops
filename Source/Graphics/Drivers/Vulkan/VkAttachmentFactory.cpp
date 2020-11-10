#include "VkAttachmentFactory.h"
#include "VulkanUtility.h"
#include "PresentationEngine.h"
#include "VulkanMemoryManager.h"
#include <RenderingWrapper.h>
#include <Assertion.h>
#include <algorithm>

VkAttachmentFactory *VkAttachmentFactory::instance = nullptr;
uint32_t VkAttachmentFactory::imageId = 0;

AttachmentInfo * VkAttachmentFactory::UnwrapImageInfo(ImageInfo * imageInfo)
{
    AttachmentInfo* vulkanImageInfo = new AttachmentInfo();

    switch (imageInfo->colorSpace)
    {
    case ColorSpace::COLOR_SPACE_SRGB_NONLINEAR_KHR:
        vulkanImageInfo->colorSpace = VK_COLOR_SPACE_SRGB_NONLINEAR_KHR;
        break;
    default:
        ASSERT_MSG(0, "Converter not found ");
        std::exit(-1);
    }

    switch (imageInfo->degree)
    {
    case Dimensions::Dim1:
        vulkanImageInfo->viewType = VK_IMAGE_VIEW_TYPE_1D;
        vulkanImageInfo->imageType = VK_IMAGE_TYPE_1D;
        break;
    case Dimensions::Dim2:
        vulkanImageInfo->viewType = VK_IMAGE_VIEW_TYPE_2D;
        vulkanImageInfo->imageType = VK_IMAGE_TYPE_2D;
        break;
    default:
        ASSERT_MSG(0, "Converter not found ");
        std::exit(-1);
    }

    switch (imageInfo->format)
    {
    case ImageFormat::UNDEFINED:
        vulkanImageInfo->format = VK_FORMAT_UNDEFINED;
        break;

    case ImageFormat::B8G8R8A8_UNORM:
        vulkanImageInfo->format = VK_FORMAT_B8G8R8A8_UNORM;
        break;

    case ImageFormat::D16_UNORM:
        vulkanImageInfo->format = VK_FORMAT_D16_UNORM;
        break;

    case ImageFormat::D16_UNORM_S8_UINT:
        vulkanImageInfo->format = VK_FORMAT_D16_UNORM_S8_UINT;
        break;

    case ImageFormat::D24_UNORM_S8_UINT:
        vulkanImageInfo->format = VK_FORMAT_D24_UNORM_S8_UINT;
        break;

    case ImageFormat::D32_SFLOAT:
        vulkanImageInfo->format = VK_FORMAT_D32_SFLOAT;
        break;

    case ImageFormat::D32_SFLOAT_S8_UINT:
        vulkanImageInfo->format = VK_FORMAT_D32_SFLOAT_S8_UINT;
        break;

    default:
        ASSERT_MSG(0, "Converter not found ");
        std::exit(-1);
    }

    vulkanImageInfo->height = imageInfo->height;
    vulkanImageInfo->layers = imageInfo->layers;
    vulkanImageInfo->mips = imageInfo->mips;
    vulkanImageInfo->width = imageInfo->width;

    vulkanImageInfo->sampleCount = VkSampleCountFlagBits(static_cast<int>(imageInfo->sampleCount));
    vulkanImageInfo->usage = VkImageUsageFlagBits(static_cast<int>(imageInfo->usage));

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

VkFormat VkAttachmentFactory::FindBestDepthFormat(VkFormat inputFormat)
{
    VkFormatProperties formatProps{};
    vkGetPhysicalDeviceFormatProperties(*CoreObjects::physicalDeviceObj, inputFormat, &formatProps);

    if (formatProps.optimalTilingFeatures & VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT)
    {
        return inputFormat;
    }
    
    std::vector<VkFormat> tryFormats{ VK_FORMAT_D32_SFLOAT_S8_UINT , VK_FORMAT_D24_UNORM_S8_UINT , VK_FORMAT_D16_UNORM_S8_UINT , VK_FORMAT_D32_SFLOAT , VK_FORMAT_D16_UNORM };
    VkFormat fallback = {};
    for (VkFormat format : tryFormats)
    {
        VkFormatProperties formatProps{};
        vkGetPhysicalDeviceFormatProperties(*CoreObjects::physicalDeviceObj, format, &formatProps);

        if (formatProps.optimalTilingFeatures & VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT)
        {
            fallback = format;
            break;
        }
    }

    return fallback;
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

void VkAttachmentFactory::CreateDepthAttachment(ImageInfo * info, uint32_t count, bool stencilRequired, bool defaultTarget, vector<uint32_t>* ids)
{
    AttachmentInfo * attachmentInfo = UnwrapImageInfo(info);
    attachmentInfo->format = FindBestDepthFormat(attachmentInfo->format);
    
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
    createInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
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
