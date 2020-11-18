#include "VkQueueFactory.h"
#include "VulkanUtility.h"
#include <Assertion.h>
#include <algorithm>
//#include <string>
VkQueueFactory* VkQueueFactory::instance = nullptr;

void VkQueueFactory::Init()
{

}

void VkQueueFactory::DeInit()
{
    delete[] priority;

    /*for each(QueueWrapper obj in queueWrapperList)
    {
        vkQueueWaitIdle(*obj.queue);
        delete obj.queue;
    }

    queueWrapperList.clear();*/
}

void VkQueueFactory::Update()
{

}

VkQueueFactory * VkQueueFactory::GetInstance()
{
    if (instance == nullptr)
    {
        instance = new VkQueueFactory();
    }
    return instance;
}

VkQueueFactory::~VkQueueFactory()
{
}

VkDeviceQueueCreateInfo VkQueueFactory::FindQueue()
{
    uint32_t uniqueQueues = 3;
    uint32_t requirementFlag = VK_QUEUE_COMPUTE_BIT | VK_QUEUE_GRAPHICS_BIT | VK_QUEUE_TRANSFER_BIT;

    uint32_t qFamilyCount = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(*CoreObjects::physicalDeviceObj, &qFamilyCount, nullptr);
    propertyList.resize(qFamilyCount);
    vkGetPhysicalDeviceQueueFamilyProperties(*CoreObjects::physicalDeviceObj, &qFamilyCount, propertyList.data());

    queueWrapperList.resize(uniqueQueues);
    VkQueue * queueList = new VkQueue[uniqueQueues];
    priority = new float[uniqueQueues] {1.0f};

    for (uint32_t j = 0; j < qFamilyCount; j++)
    {
        if (propertyList[j].queueFlags & requirementFlag)
        {
            familyIndex = j;
            break;
        }
    }

    ASSERT_MSG(familyIndex != -1, "GRAPHICS + COMPUTE + TRANSFER not available in a single family");
    ASSERT_MSG(propertyList[familyIndex].queueCount >= uniqueQueues, "Family doesnt has enough queues");

    for (uint32_t k = 0; k < uniqueQueues; k++)
    {
        uniqueIndiciesInFamily.push_back(k);
        queueWrapperList[k].queue = &queueList[k];
        queueWrapperList[k].queueFamilyIndex = familyIndex;

        if (k == 0)
            queueWrapperList[k].queueType = QueueType::GRAPHICS;
        else if(k == 1)
            queueWrapperList[k].queueType = QueueType::COMPUTE;
        else if (k == 2)
            queueWrapperList[k].queueType = QueueType::TRANSFER;
    }
    
    VkDeviceQueueCreateInfo creatInfoList = {};
    creatInfoList.flags = 0;
    creatInfoList.pNext = nullptr;
    creatInfoList.pQueuePriorities = priority;
    creatInfoList.queueCount = uniqueQueues;
    creatInfoList.queueFamilyIndex = familyIndex;
    creatInfoList.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;

    return creatInfoList;
}

void VkQueueFactory::InitQueues(uint32_t * indexInFamily, uint32_t count)
{
    for (uint32_t i = 0; i < count; i++)
    {
        vkGetDeviceQueue(*CoreObjects::logicalDeviceObj, queueWrapperList[i].queueFamilyIndex, indexInFamily[i], queueWrapperList[i].queue);
        ASSERT_MSG(*queueWrapperList[i].queue != VK_NULL_HANDLE, "Queue not available ");
    }
}

void VkQueueFactory::InitQueues()
{
    for (uint32_t i = 0; i < uniqueIndiciesInFamily.size(); i++)
    {
        vkGetDeviceQueue(*CoreObjects::logicalDeviceObj, queueWrapperList[i].queueFamilyIndex, uniqueIndiciesInFamily[i], queueWrapperList[i].queue);
        ASSERT_MSG(*queueWrapperList[i].queue != VK_NULL_HANDLE, "Queue not available ");
    }
}

VkQueue * VkQueueFactory::GetQueue(QueueType qType)
{
    std::vector<QueueWrapper>::iterator it;
    it = std::find_if(queueWrapperList.begin(), queueWrapperList.end(), [&](QueueWrapper e) { return e.queueType == qType; });

    ASSERT_MSG(it != queueWrapperList.end(), "Queue family index not found");

    return it->queue;
}

VkQueue * VkQueueFactory::GetQueue(VkQueueFlagBits qType)
{
    QueueType type;
    switch (qType)
    {
    case VK_QUEUE_GRAPHICS_BIT:
        type = QueueType::GRAPHICS;
        break;
    case VK_QUEUE_COMPUTE_BIT:
        type = QueueType::COMPUTE;
        break;
    case VK_QUEUE_TRANSFER_BIT:
        type = QueueType::TRANSFER;
        break;
    }

    std::vector<QueueWrapper>::iterator it;
    it = std::find_if(queueWrapperList.begin(), queueWrapperList.end(), [&](QueueWrapper e) { return e.queueType == type; });

    ASSERT_MSG(it != queueWrapperList.end(), "Queue family index not found");

    return it->queue;
}
