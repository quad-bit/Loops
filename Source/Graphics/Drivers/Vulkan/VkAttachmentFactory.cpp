#include "VkAttachmentFactory.h"
#include "VulkanUtility.h"

//using namespace Loops::Graphics::Vulkan;

Loops::Graphics::Vulkan::VkAttachmentFactory *Loops::Graphics::Vulkan::VkAttachmentFactory::instance = nullptr;
uint32_t Loops::Graphics::Vulkan::VkAttachmentFactory::imageId = 0;

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
}

void Loops::Graphics::Vulkan::VkAttachmentFactory::Init()
{

}

void Loops::Graphics::Vulkan::VkAttachmentFactory::DeInit()
{

}

void Loops::Graphics::Vulkan::VkAttachmentFactory::Update()
{

}

Loops::Graphics::Vulkan::VkAttachmentFactory *Loops::Graphics::Vulkan::VkAttachmentFactory::GetInstance()
{
    if (instance == nullptr)
    {
        instance = new VkAttachmentFactory();
    }
    return instance;
}

Loops::Graphics::Vulkan::VkAttachmentFactory::~VkAttachmentFactory()
{
}

void Loops::Graphics::Vulkan::VkAttachmentFactory::CreateColorAttachment(Loops::Core::Utility::Wrappers::ImageInfo * info, uint32_t count, bool defaultTarget, vector<uint32_t>* ids)
{
    vector<VkImage> * imageList;
    vector<VkImageView> * imageViewList;

    if (defaultTarget == true)
    {
        //imageList = PresentationEngine::GetInstance()->CreateSwapchainImage(UnwrapImageInfo (info), count);
        //imageViewList = PresentationEngine::GetInstance()->CreateSwapchainImageViews(UnwrapImageInfo(info), count);
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

void Loops::Graphics::Vulkan::VkAttachmentFactory::CreateDepthAttachment(Loops::Core::Utility::Wrappers::ImageInfo * info, uint32_t count, bool defaultTarget, vector<uint32_t>* ids)
{
}

void Loops::Graphics::Vulkan::VkAttachmentFactory::DestroyAttachment(vector<uint32_t> ids, bool defaultTarget)
{
    if (defaultTarget == true)
    {
        //PresentationEngine::GetInstance()->DestroySwapChainImageView();
        //PresentationEngine::GetInstance()->DestroySwapChain();
    }
}

uint32_t Loops::Graphics::Vulkan::VkAttachmentFactory::GetId()
{
    return imageId++;
}
