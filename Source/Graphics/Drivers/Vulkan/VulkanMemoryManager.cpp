#define VMA_IMPLEMENTATION

#include "VulkanMemoryManager.h"
#include "VulkanUtility.h"
#include <CorePrecompiled.h>

VulkanMemoryManager * VulkanMemoryManager::memManagerInstance = nullptr;

VulkanMemoryManager * VulkanMemoryManager::GetSingleton()
{
	if (memManagerInstance == nullptr)
		memManagerInstance = new VulkanMemoryManager;
	return memManagerInstance;
}

void VulkanMemoryManager::Init(VkPhysicalDeviceMemoryProperties physicalDeviceMemoryPropertiesObj)
{
    PLOGD << "VulkanMemoryManager init";

    this->physicalDeviceMemoryPropertiesObj = physicalDeviceMemoryPropertiesObj;
    vulkanLogicalDevice = *CoreObjects::logicalDeviceObj;

	VmaAllocatorCreateInfo allocatorInfo = {};
    allocatorInfo.physicalDevice = *CoreObjects::physicalDeviceObj;
    allocatorInfo.device = *CoreObjects::logicalDeviceObj;
	allocatorInfo.instance = *CoreObjects::instanceObj;

	vmaCreateAllocator(&allocatorInfo, &allocator);

	/*VkBufferCreateInfo bufferInfo = { VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO };
	bufferInfo.size = 65536;
	bufferInfo.usage = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT;

	VmaAllocationCreateInfo allocInfo = {};
	allocInfo.usage = VMA_MEMORY_USAGE_GPU_ONLY;

	VkBuffer buffer;
	VmaAllocation allocation;
	vmaCreateBuffer(allocator, &bufferInfo, &allocInfo, &buffer, &allocation, nullptr);
	*/


	//// Create a pool that can have at most 2 blocks, 128 MiB each.
	//VmaPoolCreateInfo poolCreateInfo = {};
	//poolCreateInfo.memoryTypeIndex = ...
	//poolCreateInfo.blockSize = 128ull * 1024 * 1024;
	//poolCreateInfo.maxBlockCount = 2;

	//VmaPool pool;
	//vmaCreatePool(allocator, &poolCreateInfo, &pool);


}

void VulkanMemoryManager::DeInit()
{
    PLOGD << "VulkanMemoryManager Deinit";
	vmaDestroyAllocator(allocator);
}

uint32_t VulkanMemoryManager::FindMemoryTypeIndex(const VkPhysicalDeviceMemoryProperties * gpu_memory_properties, const VkMemoryRequirements * memory_requirements, const VkMemoryPropertyFlags required_properties)
{
	for (uint32_t i = 0; i < gpu_memory_properties->memoryTypeCount; ++i) {
		if (memory_requirements->memoryTypeBits & (1 << i)) {
			if ((gpu_memory_properties->memoryTypes[i].propertyFlags & required_properties) == required_properties) {
				return i;
			}
		}
	}
	assert(0 && "Couldn't find proper memory type.");
	return UINT32_MAX;
}

void VulkanMemoryManager::AllocateImageMemory(VkImage * imageObj, VkMemoryPropertyFlags userReq, VkDeviceMemory * memoryObj)
{
	// image memory allocation
	// memory index..
	VkMemoryRequirements imageMemoryRequirements{};
	vkGetImageMemoryRequirements(vulkanLogicalDevice, *imageObj, &imageMemoryRequirements);

	uint32_t memIndex = VulkanMemoryManager::FindMemoryTypeIndex(&physicalDeviceMemoryPropertiesObj, &imageMemoryRequirements, userReq);
	// found memory index..

	VkMemoryAllocateInfo allocateInfo{};
	allocateInfo.allocationSize = imageMemoryRequirements.size;
	allocateInfo.memoryTypeIndex = memIndex;
	allocateInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;

	ErrorCheck(vkAllocateMemory(vulkanLogicalDevice, &allocateInfo, CoreObjects::pAllocator, memoryObj));
	// allocated memory
}

VkMemoryRequirements VulkanMemoryManager::AllocateBufferMemory(VkBuffer * bufferObj, VkMemoryPropertyFlags userReq, VkDeviceMemory * memoryObj)
{
    //buffer memory requirement
    VkMemoryRequirements memoryReqObj{};
	vkGetBufferMemoryRequirements(vulkanLogicalDevice, *bufferObj, &memoryReqObj);

	uint32_t memIndex = VulkanMemoryManager::FindMemoryTypeIndex(&physicalDeviceMemoryPropertiesObj, &memoryReqObj, userReq);
	// found memory index..

	VkMemoryAllocateInfo allocateInfoObj{};
	allocateInfoObj.allocationSize = memoryReqObj.size;
	allocateInfoObj.memoryTypeIndex = memIndex;
	allocateInfoObj.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;

	ErrorCheck(vkAllocateMemory(vulkanLogicalDevice, &allocateInfoObj, CoreObjects::pAllocator, memoryObj));

    return memoryReqObj;
}

VulkanMemoryManager::VulkanMemoryManager()
{
}

VulkanMemoryManager::~VulkanMemoryManager()
{
}
