#include "VkQueueFactory.h"
#include "VulkanUtility.h"
#include <Assertion.h>
#include <algorithm>
//#include <string>
VkQueueFactory* VkQueueFactory::instance = nullptr;


void VkQueueFactory::Init()
{
    uint32_t graphicsReq = VK_QUEUE_GRAPHICS_BIT;
    uint32_t computeReq = VK_QUEUE_COMPUTE_BIT;
    uint32_t transferReq = VK_QUEUE_TRANSFER_BIT;

    uint32_t qFamilyCount = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(*CoreObjects::physicalDeviceObj, &qFamilyCount, nullptr);
    propertyList.resize(qFamilyCount);
    vkGetPhysicalDeviceQueueFamilyProperties(*CoreObjects::physicalDeviceObj, &qFamilyCount, propertyList.data());

    for (uint32_t j = 0; j < qFamilyCount; j++)
    {
        if (graphicsQueueFamilyIndex == -1 && ((propertyList[j].queueFlags & graphicsReq) == graphicsReq)
            && propertyList[j].queueCount >= maxGraphicQueues)
        {
            graphicsQueueFamilyIndex = j;
        }

        if (computeQueueFamilyIndex == -1 && ((propertyList[j].queueFlags & computeReq) == computeReq)
            && propertyList[j].queueCount >= maxComputeQueues)
        {
            computeQueueFamilyIndex = j;
        }

        if (transferQueueFamilyIndex == -1 && ((propertyList[j].queueFlags & transferReq) == transferReq)
            && propertyList[j].queueCount >= maxTransferQueue)
        {
            transferQueueFamilyIndex = j;
        }
    }

    ASSERT_MSG(graphicsQueueFamilyIndex != -1, "min 2 graphic queues required in a family");
    ASSERT_MSG(computeQueueFamilyIndex != -1, "min 2 compute queues required in a family");
    ASSERT_MSG(transferQueueFamilyIndex != -1, "min 1 transfer queues required in a family");
}

void VkQueueFactory::DeInit()
{
    delete[] graphicQueuePriority;

    if (computeQueuePriority != nullptr)
    delete[] computeQueuePriority;

    if(transferQueuePriority != nullptr)
    delete[] transferQueuePriority;

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

std::vector<VkDeviceQueueCreateInfo> VkQueueFactory::FindQueue()
{
    /*uint32_t uniqueQueues = 3;
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

    return creatInfoList;*/
    
    graphicQueuePriority = new float[maxGraphicQueues] {1.0f};

    std::vector<VkDeviceQueueCreateInfo> creatInfoList;

    VkDeviceQueueCreateInfo info = {};
    info.flags = 0;
    info.pNext = nullptr;
    info.pQueuePriorities = graphicQueuePriority;
    info.queueCount = maxGraphicQueues;
    info.queueFamilyIndex = graphicsQueueFamilyIndex;
    info.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;

    creatInfoList.push_back(info);

    graphicsQueueWrapperList.resize(maxGraphicQueues);

    uint32_t indexInGraphicFamily = 0;
    for (uint32_t i = 0; i < maxGraphicQueues; i++)
    {
        graphicsQueueWrapperList[i].queueFamilyIndex = graphicsQueueFamilyIndex;
        graphicsQueueWrapperList[i].queueType = QueueType::GRAPHICS;
        graphicsQueueWrapperList[i].indexInFamily = indexInGraphicFamily;
        graphicsQueueWrapperList[i].queueId = GetQueueId();
        graphicsQueueWrapperList[i].queue = new VkQueue;
        indexInGraphicFamily++;
    }

    uint32_t indexInComputeFamily = -1;

    if (graphicsQueueFamilyIndex == computeQueueFamilyIndex)
    {
        creatInfoList[0].queueCount = maxGraphicQueues + maxComputeQueues;
        indexInComputeFamily = indexInGraphicFamily;
        delete [] graphicQueuePriority;
        graphicQueuePriority = new float[maxGraphicQueues + maxComputeQueues]{ 1.0f };
        creatInfoList[0].pQueuePriorities = graphicQueuePriority;
        computeQueuePriority = nullptr;
    }
    else
    {
        computeQueuePriority = new float[maxComputeQueues] {1.0f};

        VkDeviceQueueCreateInfo info = {};
        info.flags = 0;
        info.pNext = nullptr;
        info.pQueuePriorities = computeQueuePriority;
        info.queueCount = maxComputeQueues;
        info.queueFamilyIndex = computeQueueFamilyIndex;
        info.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
        creatInfoList.push_back(info);

        indexInComputeFamily = 0;
    }

    computeQueueWrapperList.resize(maxComputeQueues);
    for (uint32_t i = 0; i < maxComputeQueues; i++)
    {
        computeQueueWrapperList[i].queueFamilyIndex = computeQueueFamilyIndex;
        computeQueueWrapperList[i].queueType = QueueType::COMPUTE;
        computeQueueWrapperList[i].indexInFamily = indexInComputeFamily;
        computeQueueWrapperList[i].queueId = GetQueueId();
        computeQueueWrapperList[i].queue = new VkQueue;
        indexInComputeFamily++;
    }

    uint32_t indexInTransferFamily = -1;

    if (graphicsQueueFamilyIndex == transferQueueFamilyIndex && 
        graphicsQueueFamilyIndex == computeQueueFamilyIndex)
    {
        creatInfoList[0].queueCount = maxGraphicQueues + maxComputeQueues + maxTransferQueue;
        indexInTransferFamily = indexInComputeFamily ;
        delete[] graphicQueuePriority;
        graphicQueuePriority = new float[maxGraphicQueues + maxComputeQueues + maxTransferQueue]{ 1.0f };
        creatInfoList[0].pQueuePriorities = graphicQueuePriority;
        transferQueuePriority = nullptr;
    }
    else if(transferQueueFamilyIndex == computeQueueFamilyIndex )
    {
        creatInfoList[1].queueCount = maxComputeQueues + maxTransferQueue;
        indexInTransferFamily = indexInComputeFamily;

        delete[] computeQueuePriority;
        computeQueuePriority = new float[maxComputeQueues + maxTransferQueue]{ 1.0f };
        creatInfoList[1].pQueuePriorities = computeQueuePriority;
        transferQueuePriority = nullptr;
    }
    else if (transferQueueFamilyIndex != computeQueueFamilyIndex &&
        transferQueueFamilyIndex != graphicsQueueFamilyIndex)
    {
        transferQueuePriority = new float[maxTransferQueue] {1.0f};
        VkDeviceQueueCreateInfo info = {};
        info.flags = 0;
        info.pNext = nullptr;
        info.pQueuePriorities = transferQueuePriority;
        info.queueCount = maxTransferQueue;
        info.queueFamilyIndex = transferQueueFamilyIndex;
        info.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
        creatInfoList.push_back(info);

        indexInTransferFamily = 0;
    }

    transferQueueWrapperList.resize(maxTransferQueue);
    for (uint32_t i = 0; i < maxTransferQueue; i++)
    {
        transferQueueWrapperList[i].queueFamilyIndex = transferQueueFamilyIndex;
        transferQueueWrapperList[i].queueType = QueueType::TRANSFER;
        transferQueueWrapperList[i].indexInFamily = indexInTransferFamily;
        transferQueueWrapperList[i].queueId = GetQueueId();
        transferQueueWrapperList[i].queue = new VkQueue;
        indexInTransferFamily++;
    }
    
    return creatInfoList; 
}

//void VkQueueFactory::InitQueues(uint32_t * indexInFamily, uint32_t count)
//{
//    for (uint32_t i = 0; i < count; i++)
//    {
//        vkGetDeviceQueue(*CoreObjects::logicalDeviceObj, queueWrapperList[i].queueFamilyIndex, indexInFamily[i], queueWrapperList[i].queue);
//        ASSERT_MSG(*queueWrapperList[i].queue != VK_NULL_HANDLE, "Queue not available ");
//    }
//}

void VkQueueFactory::InitQueues()
{
    /*for (uint32_t i = 0; i < uniqueIndiciesInFamily.size(); i++)
    {
        vkGetDeviceQueue(*CoreObjects::logicalDeviceObj, queueWrapperList[i].queueFamilyIndex, uniqueIndiciesInFamily[i], queueWrapperList[i].queue);
        ASSERT_MSG(*queueWrapperList[i].queue != VK_NULL_HANDLE, "Queue not available ");
    }
*/
    for (uint32_t i = 0; i < maxGraphicQueues; i++)
    {
        vkGetDeviceQueue(*CoreObjects::logicalDeviceObj, graphicsQueueWrapperList[i].queueFamilyIndex,
            graphicsQueueWrapperList[i].indexInFamily, graphicsQueueWrapperList[i].queue);
        ASSERT_MSG(*graphicsQueueWrapperList[i].queue != VK_NULL_HANDLE, "Graphics Queue not available ");
    }

    for (uint32_t i = 0; i < maxComputeQueues; i++)
    {
        vkGetDeviceQueue(*CoreObjects::logicalDeviceObj, computeQueueWrapperList[i].queueFamilyIndex,
            computeQueueWrapperList[i].indexInFamily, computeQueueWrapperList[i].queue);
        ASSERT_MSG(*computeQueueWrapperList[i].queue != VK_NULL_HANDLE, "Compute Queue not available ");
    }

    for (uint32_t i = 0; i < maxTransferQueue; i++)
    {
        vkGetDeviceQueue(*CoreObjects::logicalDeviceObj, transferQueueWrapperList[i].queueFamilyIndex, 
            transferQueueWrapperList[i].indexInFamily, transferQueueWrapperList[i].queue);
        ASSERT_MSG(*transferQueueWrapperList[i].queue != VK_NULL_HANDLE, "Transfer Queue not available ");
    }
}

VkQueue * VkQueueFactory::GetQueue(QueueType qType, uint32_t id)
{
    std::vector<QueueWrapper>::iterator it;

    switch (qType)
    {
    case QueueType::GRAPHICS:
        {
            it = std::find_if(graphicsQueueWrapperList.begin(), graphicsQueueWrapperList.end(), [&](QueueWrapper e) { return e.queueId== id; });
            ASSERT_MSG(it != graphicsQueueWrapperList.end(), "Queue family index not found");

            return it->queue;
        }
        break;

    case QueueType::COMPUTE:
        {
            it = std::find_if(computeQueueWrapperList.begin(), computeQueueWrapperList.end(), [&](QueueWrapper e) { return e.queueId == id; });
            ASSERT_MSG(it != computeQueueWrapperList.end(), "Queue family index not found");

            return it->queue;
        }
        break;

    case QueueType::TRANSFER:
        {
            it = std::find_if(transferQueueWrapperList.begin(), transferQueueWrapperList.end(), [&](QueueWrapper e) { return e.queueId == id; });
            ASSERT_MSG(it != transferQueueWrapperList.end(), "Queue family index not found");

            return it->queue;
        }
        break;

    default:
        ASSERT_MSG(0, "Queue not found");
    }

    return nullptr;
}

VkQueue * VkQueueFactory::GetQueue(VkQueueFlagBits qType, uint32_t id)
{
    std::vector<QueueWrapper>::iterator it;

    switch (qType)
    {
    case VkQueueFlagBits::VK_QUEUE_GRAPHICS_BIT:
    {
        it = std::find_if(graphicsQueueWrapperList.begin(), graphicsQueueWrapperList.end(), [&](QueueWrapper e) { return e.queueId == id; });
        ASSERT_MSG(it != graphicsQueueWrapperList.end(), "Queue family index not found");

        return it->queue;
    }
    break;

    case VkQueueFlagBits::VK_QUEUE_COMPUTE_BIT:
    {
        it = std::find_if(computeQueueWrapperList.begin(), computeQueueWrapperList.end(), [&](QueueWrapper e) { return e.queueId == id; });
        ASSERT_MSG(it != computeQueueWrapperList.end(), "Queue family index not found");

        return it->queue;
    }
    break;

    case VkQueueFlagBits::VK_QUEUE_TRANSFER_BIT:
    {
        it = std::find_if(transferQueueWrapperList.begin(), transferQueueWrapperList.end(), [&](QueueWrapper e) { return e.queueId == id; });
        ASSERT_MSG(it != transferQueueWrapperList.end(), "Queue family index not found");

        return it->queue;
    }
    break;

    default:
        ASSERT_MSG(0, "Queue not found");
    }
    return nullptr;
}

uint32_t VkQueueFactory::GetQueueFamilyIndex(QueueType qType, uint32_t queueId)
{
    std::vector<QueueWrapper>::iterator it;

    switch (qType)
    {
    case QueueType::GRAPHICS:
    {
        it = std::find_if(graphicsQueueWrapperList.begin(), graphicsQueueWrapperList.end(), [&](QueueWrapper e) { return e.queueId == queueId; });
        ASSERT_MSG(it != graphicsQueueWrapperList.end(), "Queue family index not found");

        return it->queueFamilyIndex;
    }
    break;

    case QueueType::COMPUTE:
    {
        it = std::find_if(computeQueueWrapperList.begin(), computeQueueWrapperList.end(), [&](QueueWrapper e) { return e.queueId == queueId; });
        ASSERT_MSG(it != computeQueueWrapperList.end(), "Queue family index not found");

        return it->queueFamilyIndex;
    }
    break;

    case QueueType::TRANSFER:
    {
        it = std::find_if(transferQueueWrapperList.begin(), transferQueueWrapperList.end(), [&](QueueWrapper e) { return e.queueId == queueId; });
        ASSERT_MSG(it != transferQueueWrapperList.end(), "Queue family index not found");

        return it->queueFamilyIndex;
    }
    break;

    default:
        ASSERT_MSG(0, "Queue not found");
    }

    return -1;
}

uint32_t VkQueueFactory::GetQueueFamilyIndex(VkQueueFlagBits qType, uint32_t queueId)
{
    std::vector<QueueWrapper>::iterator it;

    switch (qType)
    {
    case VkQueueFlagBits::VK_QUEUE_GRAPHICS_BIT:
    {
        it = std::find_if(graphicsQueueWrapperList.begin(), graphicsQueueWrapperList.end(), [&](QueueWrapper e) { return e.queueId == queueId; });
        ASSERT_MSG(it != graphicsQueueWrapperList.end(), "Queue family index not found");

        return it->queueFamilyIndex;
    }
    break;

    case VkQueueFlagBits::VK_QUEUE_COMPUTE_BIT:
    {
        it = std::find_if(computeQueueWrapperList.begin(), computeQueueWrapperList.end(), [&](QueueWrapper e) { return e.queueId == queueId; });
        ASSERT_MSG(it != computeQueueWrapperList.end(), "Queue family index not found");

        return it->queueFamilyIndex;
    }
    break;

    case VkQueueFlagBits::VK_QUEUE_TRANSFER_BIT:
    {
        it = std::find_if(transferQueueWrapperList.begin(), transferQueueWrapperList.end(), [&](QueueWrapper e) { return e.queueId == queueId; });
        ASSERT_MSG(it != transferQueueWrapperList.end(), "Queue family index not found");

        return it->queueFamilyIndex;
    }
    break;

    default:
        ASSERT_MSG(0, "Queue not found");
    }
    return -1;
}

void VkQueueFactory::CreateGraphicsQueues(uint32_t * ids, const uint32_t & count)
{
    ASSERT_MSG(count <= maxGraphicQueues, "Not enough Compute Queue");
    
    for (uint32_t i = 0; i < count; i++)
    {
        ASSERT_MSG(graphicQueueInitCounter <= maxGraphicQueues, "Graphics Queue exhausted ");

        vkGetDeviceQueue(*CoreObjects::logicalDeviceObj, graphicsQueueWrapperList[graphicQueueInitCounter].queueFamilyIndex,
            graphicsQueueWrapperList[graphicQueueInitCounter].indexInFamily, graphicsQueueWrapperList[graphicQueueInitCounter].queue);
        ASSERT_MSG(*graphicsQueueWrapperList[graphicQueueInitCounter].queue != VK_NULL_HANDLE, "Graphics Queue not available ");

        ids[i] = graphicsQueueWrapperList[graphicQueueInitCounter].queueId;
        graphicQueueInitCounter++;
    }
}

void VkQueueFactory::CreateComputeQueues(uint32_t * ids, const uint32_t & count)
{
    ASSERT_MSG(count <= maxComputeQueues, "Not enough Compute Queue");
    for (uint32_t i = 0; i < count; i++)
    {
        ASSERT_MSG(computeQueueInitCounter <= maxComputeQueues, "Compute Queue exhausted ");

        vkGetDeviceQueue(*CoreObjects::logicalDeviceObj, computeQueueWrapperList[computeQueueInitCounter].queueFamilyIndex,
            computeQueueWrapperList[computeQueueInitCounter].indexInFamily, computeQueueWrapperList[computeQueueInitCounter].queue);
        ASSERT_MSG(*computeQueueWrapperList[computeQueueInitCounter].queue != VK_NULL_HANDLE, "Compute Queue not available ");

        ids[i] = computeQueueWrapperList[computeQueueInitCounter].queueId;
        computeQueueInitCounter++;
    }
}

void VkQueueFactory::CreateTransferQueues(uint32_t * ids, const uint32_t & count)
{
    ASSERT_MSG(count <= maxTransferQueue, "Not enough transfer Queue");

    for (uint32_t i = 0; i < count; i++)
    {
        ASSERT_MSG(transferQueueInitCounter <= maxTransferQueue, "Compute Queue exhausted ");

        vkGetDeviceQueue(*CoreObjects::logicalDeviceObj, transferQueueWrapperList[transferQueueInitCounter].queueFamilyIndex,
            transferQueueWrapperList[transferQueueInitCounter].indexInFamily, transferQueueWrapperList[transferQueueInitCounter].queue);
        ASSERT_MSG(*transferQueueWrapperList[transferQueueInitCounter].queue != VK_NULL_HANDLE, "Transfer Queue not available ");
        ids[i] = transferQueueWrapperList[transferQueueInitCounter].queueId;
        transferQueueInitCounter++;
    }
}

