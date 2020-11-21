#include "VkCommandBufferFactory.h"
#include <algorithm>
#include <Assertion.h>
#include <VulkanUtility.h>
#include <VkQueueFactory.h>
#include "VkDrawCommandBuffer.h"

VkCommandBufferFactory* VkCommandBufferFactory::instance = nullptr;
uint32_t VkCommandBufferFactory::poolIdCounter = 0, VkCommandBufferFactory::bufferIdCounter = 0;

VkCommandBuffer * VkCommandBufferFactory::GetCommandBuffer(uint32_t id)
{
    std::vector<VkDrawCommandBuffer*>::iterator it;
    it = std::find_if(drawCommandBufferList.begin(), drawCommandBufferList.end(), [&](VkDrawCommandBuffer* e) { return e->id == id; });

    ASSERT_MSG(it != drawCommandBufferList.end(), "Image id not found");
    
    return (*it)->commandBuffer;
}

VkCommandPool * VkCommandBufferFactory::GetCommandPool(uint32_t poolId)
{
    std::vector<VkCommandPoolWrapper>::iterator it;
    it = std::find_if(poolList.begin(), poolList.end(), [&](VkCommandPoolWrapper e) { return e.poolId == poolId; });

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
    for each (VkCommandPoolWrapper pool in poolList)
    {
        delete pool.pool;
    }

    poolList.clear();

    for each (VkDrawCommandBuffer* buf in drawCommandBufferList)
    {
        delete buf->commandBuffer;
    }

    drawCommandBufferList.clear();
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

    VkCommandPoolWrapper wrapper;
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

/*
void VkCommandBufferFactory::CreateCommandBuffer(uint32_t poolId, VkCommandBufferLevel level, uint32_t count, uint32_t * ids)
{
    VkCommandPool* pool = GetCommandPool(poolId);
    VkCommandBufferWrapper * wrapper = new VkCommandBufferWrapper;
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
*/

VkDrawCommandBuffer * VkCommandBufferFactory::CreateCommandBuffer(uint32_t poolId, VkCommandBufferLevel level, 
    PipelineType commandBufferType, uint32_t * ids)
{
    VkCommandPool* pool = GetCommandPool(poolId);
    VkDrawCommandBuffer * drawCommandBuffer = new VkDrawCommandBuffer;
    
    drawCommandBuffer->commandBuffer = new VkCommandBuffer;
    drawCommandBuffer->id = GetBufferId();
    drawCommandBuffer->pool = pool;
    drawCommandBuffer->commandBufferType = commandBufferType;
    *ids = drawCommandBuffer->id;

    VkCommandBufferAllocateInfo info = {};
    info.commandBufferCount = 1;
    info.level = level;
    info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    info.commandPool = *pool;

    ErrorCheck(vkAllocateCommandBuffers(*CoreObjects::logicalDeviceObj,
        &info, drawCommandBuffer->commandBuffer));

    drawCommandBufferList.push_back(drawCommandBuffer);

    return drawCommandBuffer;
}

void VkCommandBufferFactory::DestroyCommandPool(uint32_t id)
{
    std::vector<VkCommandPoolWrapper>::iterator it;
    it = std::find_if(poolList.begin(), poolList.end(), [&](VkCommandPoolWrapper e) { return e.poolId == id; });

    ASSERT_MSG(it != poolList.end(), "Pool id not found");

    vkDestroyCommandPool(*CoreObjects::logicalDeviceObj, *it->pool, CoreObjects::pAllocator);
}

void VkCommandBufferFactory::ResetCommandPool(uint32_t id)
{
}

void VkCommandBufferFactory::ResetCommandBuffer(uint32_t poolId, uint32_t id)
{

    std::vector<VkDrawCommandBuffer*>::iterator it;
    it = std::find_if(drawCommandBufferList.begin(), drawCommandBufferList.end(), [&](VkDrawCommandBuffer * e) { return e->id == id; });

    ASSERT_MSG(it != drawCommandBufferList.end(), "Pool id not found");

    vkResetCommandBuffer(*(*it)->commandBuffer, VkCommandBufferResetFlagBits::VK_COMMAND_BUFFER_RESET_RELEASE_RESOURCES_BIT);

}

void VkCommandBufferFactory::DestroyCommandBuffer(uint32_t id)
{
    std::vector<VkDrawCommandBuffer*>::iterator it;
    it = std::find_if(drawCommandBufferList.begin(), drawCommandBufferList.end(), [&](VkDrawCommandBuffer * e) { return e->id == id; });

    ASSERT_MSG(it != drawCommandBufferList.end(), "Pool id not found");

    vkFreeCommandBuffers(*CoreObjects::logicalDeviceObj, *(*it)->pool, 1, (*it)->commandBuffer);
}
