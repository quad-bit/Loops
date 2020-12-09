#include "VkBufferFactory.h"
#include "VulkanUtility.h"
#include "VulkanSettings.h"
#include "VulkanMemoryManager.h"
#include <Assertion.h>
#include <algorithm>

VkBufferFactory* VkBufferFactory::instance = nullptr;

uint32_t VkBufferFactory::GetId()
{
    return idCounter++;
}

void VkBufferFactory::Init()
{

}

void VkBufferFactory::DeInit()
{

}

void VkBufferFactory::Update()
{

}

VkBufferFactory * VkBufferFactory::GetInstance()
{
    if (instance == nullptr)
    {
        instance = new VkBufferFactory();
    }
    return instance;
}

VkBufferFactory::~VkBufferFactory()
{
}

uint32_t * VkBufferFactory::CreateBuffer(VkBufferUsageFlags * bufferType,
    VkMemoryPropertyFlags * memType, void * data, size_t dataSize, void * pGpuMem)
{
    {
    BufferWrapper bufWrapper = {};
    bufWrapper.buffer = new VkBuffer;
    bufWrapper.id = GetId();
    bufWrapper.bufferMemory = new VkDeviceMemory;
    bufWrapper.bufferType = bufferType;
    bufWrapper.pGpuMem = nullptr;

    VkBufferCreateInfo bufferCreateInfo{};
    bufferCreateInfo.flags = 0;
    bufferCreateInfo.pQueueFamilyIndices = nullptr;
    bufferCreateInfo.queueFamilyIndexCount = 0;
    bufferCreateInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
    bufferCreateInfo.size = dataSize;
    bufferCreateInfo.usage = *bufferType;
    bufferCreateInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;

    ErrorCheck(vkCreateBuffer(*CoreObjects::logicalDeviceObj, &bufferCreateInfo, CoreObjects::pAllocator, bufWrapper.buffer));

    VkMemoryRequirements memoryReqObj = VulkanMemoryManager::GetSingleton()->AllocateBufferMemory(bufWrapper.buffer, *memType, bufWrapper.bufferMemory);

    ErrorCheck(vkMapMemory(*CoreObjects::logicalDeviceObj, *bufWrapper.bufferMemory, 0,
        memoryReqObj.size, 0, (void**)&bufWrapper.pGpuMem));
    memcpy(bufWrapper.pGpuMem, data, dataSize);

    vkUnmapMemory(*CoreObjects::logicalDeviceObj, *bufWrapper.bufferMemory);

    vkBindBufferMemory(*CoreObjects::logicalDeviceObj, *bufWrapper.buffer, *bufWrapper.bufferMemory, 0);

    bufferWrapperList.push_back(bufWrapper);
    pGpuMem = bufferWrapperList[bufferWrapperList.size() - 1].pGpuMem;
    }
    return &bufferWrapperList[bufferWrapperList.size() - 1].id;
}

void VkBufferFactory::DestroyBuffer(uint32_t id)
{
    std::vector<BufferWrapper>::iterator it;
    it = std::find_if(bufferWrapperList.begin(), bufferWrapperList.end(), [&](BufferWrapper e) { return e.id == id; });

    ASSERT_MSG(it != bufferWrapperList.end(), "Image id not found");
    
    vkFreeMemory(*CoreObjects::logicalDeviceObj, *it->bufferMemory, CoreObjects::pAllocator);
    vkDestroyBuffer(*CoreObjects::logicalDeviceObj, *it->buffer, CoreObjects::pAllocator);

    delete it->bufferMemory;
    delete it->bufferType;
    delete it->buffer;

}
