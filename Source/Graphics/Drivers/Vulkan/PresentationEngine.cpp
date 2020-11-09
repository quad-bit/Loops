#include "PresentationEngine.h"
#include "VkAttachmentFactory.h"
#include "VulkanUtility.h"
#include <Settings.h>
#include <vector>

using namespace std;

PresentationEngine* PresentationEngine::instance = nullptr;

void PresentationEngine::Init(VkSurfaceKHR* surfaceObj, VkSurfaceFormatKHR * surfaceFormat)
{
    vkGetPhysicalDeviceSurfaceCapabilitiesKHR(*CoreObjects::physicalDeviceObj, *surfaceObj, &surfaceCapabilities);

    if (surfaceCapabilities.maxImageCount > 0)
        if (swapChainImageCount > surfaceCapabilities.maxImageCount)
            swapChainImageCount = surfaceCapabilities.maxImageCount;

    if (swapChainImageCount < surfaceCapabilities.minImageCount + 1)
        swapChainImageCount = surfaceCapabilities.minImageCount + 1;

    VkPresentModeKHR presentMode = VkPresentModeKHR::VK_PRESENT_MODE_FIFO_KHR;
    {
        uint32_t count = 0;
        ErrorCheck(vkGetPhysicalDeviceSurfacePresentModesKHR(*CoreObjects::physicalDeviceObj, *surfaceObj, &count, nullptr));
        std::vector<VkPresentModeKHR> presentModeList(count);
        ErrorCheck(vkGetPhysicalDeviceSurfacePresentModesKHR(*CoreObjects::physicalDeviceObj, *surfaceObj, &count, presentModeList.data()));

        for (VkPresentModeKHR obj : presentModeList)
        {
            if (obj == VK_PRESENT_MODE_MAILBOX_KHR)
            {
                presentMode = obj;
                swapChainImageCount = 3;
                break;
            }
        }
    }

    Settings::swapBufferCount = swapChainImageCount;
}

vector<VkImage>* PresentationEngine::CreateSwapchainImage(AttachmentInfo * info, uint32_t count)
{
    VkSwapchainCreateInfoKHR swapChainCreateInfo{};
    swapChainCreateInfo.clipped = VK_TRUE; // dont render parts of swapchain image that are out of the frustrum
    swapChainCreateInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
    swapChainCreateInfo.imageArrayLayers = 1; // 2 meant for sterescopic rendering
    swapChainCreateInfo.imageColorSpace = surfaceFormat->colorSpace;
    swapChainCreateInfo.imageExtent = surfaceCapabilities.currentExtent;
    swapChainCreateInfo.imageFormat = surfaceFormat->format;
    swapChainCreateInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
    swapChainCreateInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
    swapChainCreateInfo.minImageCount = swapChainImageCount;
    swapChainCreateInfo.preTransform = VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR;
    swapChainCreateInfo.presentMode = presentMode;
    swapChainCreateInfo.oldSwapchain = VK_NULL_HANDLE; // useful when resizing the window
    swapChainCreateInfo.queueFamilyIndexCount = 0; // as its not shared between multiple queues
    swapChainCreateInfo.pQueueFamilyIndices = nullptr;
    swapChainCreateInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
    swapChainCreateInfo.surface = *surfaceObj;

    ErrorCheck(vkCreateSwapchainKHR(*CoreObjects::logicalDeviceObj, &swapChainCreateInfo, CoreObjects::pAllocator, &swapchainObj));

    ErrorCheck(vkGetSwapchainImagesKHR(*CoreObjects::logicalDeviceObj, swapchainObj, &swapChainImageCount, nullptr));

    swapChainImageList.resize(swapChainImageCount);
    swapChainImageViewList.resize(swapChainImageCount);

    ErrorCheck(vkGetSwapchainImagesKHR(*CoreObjects::logicalDeviceObj, swapchainObj, &swapChainImageCount, swapChainImageList.data()));

    return &swapChainImageList;
}

vector<VkImageView>* PresentationEngine::CreateSwapchainImageViews(AttachmentInfo * info, uint32_t count)
{
    for (uint32_t i = 0; i <swapChainImageCount; i++)
    {
        VkImageViewCreateInfo createInfo{};
        createInfo.components = { VK_COMPONENT_SWIZZLE_IDENTITY,VK_COMPONENT_SWIZZLE_IDENTITY,VK_COMPONENT_SWIZZLE_IDENTITY,VK_COMPONENT_SWIZZLE_IDENTITY };
        createInfo.format = surfaceFormat->format;
        createInfo.image = swapChainImageList[i];
        createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
        createInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        createInfo.subresourceRange.baseArrayLayer = 0;
        createInfo.subresourceRange.baseMipLevel = 0;
        createInfo.subresourceRange.layerCount = 1;
        createInfo.subresourceRange.levelCount = 1;
        createInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;

        ErrorCheck(vkCreateImageView(*CoreObjects::logicalDeviceObj, &createInfo, CoreObjects::pAllocator, &swapChainImageViewList[i]));
    }

    return &swapChainImageViewList;
}

void PresentationEngine::DestroySwapChain()
{
    vkDestroySwapchainKHR(*CoreObjects::logicalDeviceObj, swapchainObj, CoreObjects::pAllocator);
}

void PresentationEngine::DestroySwapChainImageView()
{
    for (uint32_t i = 0; i <swapChainImageCount; i++)
    {
        vkDestroyImageView(*CoreObjects::logicalDeviceObj, swapChainImageViewList[i], CoreObjects::pAllocator);
    }
}

void PresentationEngine::DeInit()
{
    
}

void PresentationEngine::Update()
{

}

PresentationEngine * PresentationEngine::GetInstance()
{
    if (instance == nullptr)
    {
        instance = new PresentationEngine();
    }
    return instance;
}

PresentationEngine::~PresentationEngine()
{

}
