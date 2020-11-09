#include "VkAttachmentFactory.h"
#include "VulkanUtility.h"
#include "PresentationEngine.h"
#include <RenderingWrapper.h>

VkAttachmentFactory *VkAttachmentFactory::instance = nullptr;
uint32_t VkAttachmentFactory::imageId = 0;

/*
void Loops::Graphics::Vulkan::VkAttachmentFactory::CreateSwapChainImages()
{
}

void Loops::Graphics::Vulkan::VkAttachmentFactory::CreateSwapChainImageViews()
{
}

void Loops::Graphics::Vulkan::VkAttachmentFactory::DestroySwapChainImages()
{
}

void Loops::Graphics::Vulkan::VkAttachmentFactory::DestroySwapChainImageViews()
{
}*/

AttachmentInfo * VkAttachmentFactory::UnwrapImageInfo(ImageInfo * imageInfo)
{
    return nullptr;
}

void VkAttachmentFactory::Init()
{

}

void VkAttachmentFactory::DeInit()
{

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

void VkAttachmentFactory::CreateColorAttachment(ImageInfo * info, uint32_t count, bool defaultTarget, vector<uint32_t>* ids)
{
    vector<VkImage> * imageList;
    vector<VkImageView> * imageViewList;

    if (defaultTarget == true)
    {
        imageList = PresentationEngine::GetInstance()->CreateSwapchainImage(UnwrapImageInfo (info), count);
        imageViewList = PresentationEngine::GetInstance()->CreateSwapchainImageViews(UnwrapImageInfo(info), count);
    }

    for (uint32_t i = 0; i < count; i++)
    {
        AttachmentWrapper temp = {};
        temp.id = GetId();
        temp.image = &imageList->at(i);
        temp.imageView = &imageViewList->at(i);

        ids->push_back(temp.id);
        attachmentList.push_back(temp);
    }
}

void VkAttachmentFactory::CreateDepthAttachment(ImageInfo * info, uint32_t count, bool defaultTarget, vector<uint32_t>* ids)
{
}

void VkAttachmentFactory::DestroyAttachment(vector<uint32_t> ids, bool defaultTarget)
{
    if (defaultTarget == true)
    {
        PresentationEngine::GetInstance()->DestroySwapChainImageView();
        PresentationEngine::GetInstance()->DestroySwapChain();
    }
}

uint32_t VkAttachmentFactory::GetId()
{
    return imageId++;
}
