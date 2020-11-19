#include "VkSynchroniserFactory.h"
#include "VulkanUtility.h"
#include <algorithm>
#include <Assertion.h>

VkSynchroniserFactory* VkSynchroniserFactory::instance = nullptr;

void VkSynchroniserFactory::Init()
{

}

void VkSynchroniserFactory::DeInit()
{
    for each(FenceWrapper wrapper in fenceList)
    {
        if (wrapper.fence != nullptr)
            delete wrapper.fence;
    }

    fenceList.clear();

    for each(SemaphoreWrapper wrapper in semaphoreList)
    {
        if (wrapper.semaphore != nullptr)
            delete wrapper.semaphore;
    }

    semaphoreList.clear();
}

void VkSynchroniserFactory::Update()
{

}

VkSynchroniserFactory * VkSynchroniserFactory::GetInstance()
{
    if (instance == nullptr)
    {
        instance = new VkSynchroniserFactory();
    }
    return instance;
}

VkSynchroniserFactory::~VkSynchroniserFactory()
{
}

uint32_t VkSynchroniserFactory::CreateFence(bool isFenceSignaled)
{
    FenceWrapper wrapper;
    wrapper.fence = new VkFence;
    wrapper.id = GetFenceId();

    VkFenceCreateInfo info = {};
    if (isFenceSignaled)
        info.flags = VK_FENCE_CREATE_SIGNALED_BIT;
    info.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;

    ErrorCheck(vkCreateFence(*CoreObjects::logicalDeviceObj, &info, 
        CoreObjects::pAllocator, wrapper.fence));

    fenceList.push_back(wrapper);

    return wrapper.id;
}

void VkSynchroniserFactory::DestroyFence(uint32_t id)
{
    vkDestroyFence(*CoreObjects::logicalDeviceObj, *GetFence(id), CoreObjects::pAllocator);
}

VkFence * VkSynchroniserFactory::GetFence(uint32_t id)
{
    std::vector<FenceWrapper>::iterator it;
    it = std::find_if(fenceList.begin(), fenceList.end(), [&](FenceWrapper e) { return e.id == id; });

    ASSERT_MSG(it != fenceList.end(), "Image id not found");
    return it->fence;
}

uint32_t VkSynchroniserFactory::Create_Semaphore(bool isSemaphoreSignaled)
{
    SemaphoreWrapper wrapper;
    wrapper.semaphore = new VkSemaphore;
    wrapper.id = GetSemaphoreId();
    
    VkSemaphoreCreateInfo info = {};
    if (isSemaphoreSignaled)
        info.flags = VK_FENCE_CREATE_SIGNALED_BIT;
    info.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

    ErrorCheck(vkCreateSemaphore(*CoreObjects::logicalDeviceObj, &info,
        CoreObjects::pAllocator, wrapper.semaphore));

    semaphoreList.push_back(wrapper);

    return wrapper.id;
}

void VkSynchroniserFactory::DestroySemaphore(uint32_t id)
{
    vkDestroySemaphore(*CoreObjects::logicalDeviceObj, *GetSemaphore(id), CoreObjects::pAllocator);
}

VkSemaphore* VkSynchroniserFactory::GetSemaphore(uint32_t id)
{
    std::vector<SemaphoreWrapper>::iterator it;
    it = std::find_if(semaphoreList.begin(), semaphoreList.end(), [&](SemaphoreWrapper e) { return e.id == id; });

    ASSERT_MSG(it != semaphoreList.end(), "Image id not found");
    return it->semaphore;
}
