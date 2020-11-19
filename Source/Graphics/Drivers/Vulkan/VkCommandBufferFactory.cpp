#include "VkCommandBufferFactory.h"
#include <algorithm>
#include <Assertion.h>
#include <VulkanUtility.h>
#include <VkQueueFactory.h>

VkCommandBufferFactory* VkCommandBufferFactory::instance = nullptr;
uint32_t VkCommandBufferFactory::poolIdCounter = 0, VkCommandBufferFactory::bufferIdCounter = 0;

VkCommandBuffer * VkCommandBufferFactory::GetCommandBuffer(uint32_t id)
{
    std::vector<CommandBufferWrapper>::iterator it;
    it = std::find_if(bufferList.begin(), bufferList.end(), [&](CommandBufferWrapper e) { return e.id == id; });

    ASSERT_MSG(it != bufferList.end(), "Image id not found");
    
    return it->commandBuffer;
}

VkCommandPool * VkCommandBufferFactory::GetCommandPool(uint32_t poolId)
{
    std::vector<CommandPoolWrapper>::iterator it;
    it = std::find_if(poolList.begin(), poolList.end(), [&](CommandPoolWrapper e) { return e.poolId == poolId; });

    ASSERT_MSG(it != poolList.end(), "Image id not found");

    return it->pool;
}

uint32_t VkCommandBufferFactory::GetPoolId()
{
    return poolIdCounter++;
}

uint32_t VkCommandBufferFactory::GetBufferId()
{
    return bufferIdCounter++;
}

void VkCommandBufferFactory::Init()
{
    //VkQueueFactory::GetInstance()->CreateGraphicsQueues(&renderQueueId, 1);
    //VkQueueFactory::GetInstance()->CreateComputeQueues(&computeQueueId, 1);
    //VkQueueFactory::GetInstance()->CreateTransferQueues(&transferQueueId, 1);

    renderQueue = VkQueueFactory::GetInstance()->GetQueue(VkQueueFlagBits::VK_QUEUE_GRAPHICS_BIT, CoreObjects::renderQueueId);
    computeQueue = VkQueueFactory::GetInstance()->GetQueue(VkQueueFlagBits::VK_QUEUE_COMPUTE_BIT, CoreObjects::computeQueueId);
    transferQueue = VkQueueFactory::GetInstance()->GetQueue(VkQueueFlagBits::VK_QUEUE_TRANSFER_BIT, CoreObjects::transferQueueId);

    graphicsQueueFamilyId = VkQueueFactory::GetInstance()->GetQueueFamilyIndex(VkQueueFlagBits::VK_QUEUE_GRAPHICS_BIT, CoreObjects::renderQueueId);
    computeQueueFamilyId = VkQueueFactory::GetInstance()->GetQueueFamilyIndex(VkQueueFlagBits::VK_QUEUE_COMPUTE_BIT, CoreObjects::computeQueueId);
    transferQueueFamilyId = VkQueueFactory::GetInstance()->GetQueueFamilyIndex(VkQueueFlagBits::VK_QUEUE_TRANSFER_BIT, CoreObjects::transferQueueId);
}

void VkCommandBufferFactory::DeInit()
{
    for each (CommandPoolWrapper pool in poolList)
    {
        delete pool.pool;
    }

    poolList.clear();

    for each (CommandBufferWrapper buf in bufferList)
    {
        delete buf.commandBuffer;
    }

    bufferList.clear();
}

void VkCommandBufferFactory::Update()
{

}

VkCommandBufferFactory * VkCommandBufferFactory::GetInstance()
{
    if (instance == nullptr)
    {
        instance = new VkCommandBufferFactory();
    }
    return instance;
}

VkCommandBufferFactory::~VkCommandBufferFactory()
{
}

void VkCommandBufferFactory::CreateCommandPool(VkCommandPoolCreateFlagBits flags, VkQueueFlagBits queueType, uint32_t & id)
{
    uint32_t queueFamily = -1;

    switch (queueType)
    {
    case VkQueueFlagBits::VK_QUEUE_GRAPHICS_BIT :
        queueFamily = graphicsQueueFamilyId;
        break;

    case VkQueueFlagBits::VK_QUEUE_COMPUTE_BIT:
        queueFamily = computeQueueFamilyId;
        break;

    case VkQueueFlagBits::VK_QUEUE_TRANSFER_BIT:
        queueFamily = transferQueueFamilyId;
        break;

    default:
        ASSERT_MSG(0, "Command buffer for this queue not supported");
    }

    CommandPoolWrapper wrapper;
    wrapper.poolId = GetPoolId();
    wrapper.pool = new VkCommandPool;
    
    VkCommandPoolCreateInfo info = {};
    info.queueFamilyIndex = queueFamily;
    info.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
    info.flags = flags;

    ErrorCheck(vkCreateCommandPool(*CoreObjects::logicalDeviceObj, &info, 
        CoreObjects::pAllocator, wrapper.pool));

    id = wrapper.poolId;
    poolList.push_back(wrapper);
}

void VkCommandBufferFactory::CreateCommandBuffer(uint32_t poolId, VkCommandBufferLevel level, uint32_t count, uint32_t * ids)
{
    VkCommandPool* pool = GetCommandPool(poolId);
    CommandBufferWrapper * wrapper = new CommandBufferWrapper;
    wrapper->commandBuffer = new VkCommandBuffer;
    wrapper->id = GetBufferId();
    wrapper->pool = pool;

    ids[0] = wrapper->id;

    VkCommandBufferAllocateInfo info = {};
    info.commandBufferCount = count;
    info.level = level;
    info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    info.commandPool = *pool;

    ErrorCheck(vkAllocateCommandBuffers(*CoreObjects::logicalDeviceObj, 
        &info, wrapper->commandBuffer));

    bufferList.push_back(*wrapper);
}

void VkCommandBufferFactory::DestroyCommandPool(uint32_t id)
{
    std::vector<CommandPoolWrapper>::iterator it;
    it = std::find_if(poolList.begin(), poolList.end(), [&](CommandPoolWrapper e) { return e.poolId == id; });

    ASSERT_MSG(it != poolList.end(), "Pool id not found");

    vkDestroyCommandPool(*CoreObjects::logicalDeviceObj, *it->pool, CoreObjects::pAllocator);
}

void VkCommandBufferFactory::ResetCommandPool(uint32_t id)
{
}

void VkCommandBufferFactory::ResetCommandBuffer(uint32_t poolId, uint32_t id)
{
}

void VkCommandBufferFactory::DestroyCommandBuffer(uint32_t id)
{
    std::vector<CommandBufferWrapper>::iterator it;
    it = std::find_if(bufferList.begin(), bufferList.end(), [&](CommandBufferWrapper e) { return e.id == id; });

    ASSERT_MSG(it != bufferList.end(), "Pool id not found");

    vkFreeCommandBuffers(*CoreObjects::logicalDeviceObj, *it->pool, 1, it->commandBuffer);
}
