#pragma once
#include <vulkan\vulkan.h>
#include "vk_mem_alloc.h"

class VulkanManager;
	
class VulkanMemoryManager
{
private:
	VulkanMemoryManager();
	VulkanMemoryManager(VulkanMemoryManager const &) {};
	VulkanMemoryManager const & operator=(VulkanMemoryManager const &) {};
	static VulkanMemoryManager* memManagerInstance;

	VkPhysicalDeviceMemoryProperties physicalDeviceMemoryPropertiesObj{};
	VmaAllocator allocator;
	VkDevice vulkanLogicalDevice;
	uint32_t FindMemoryTypeIndex(const VkPhysicalDeviceMemoryProperties * gpu_memory_properties, const VkMemoryRequirements * memory_requirements, const VkMemoryPropertyFlags required_properties);

public:
	~VulkanMemoryManager();
	static VulkanMemoryManager* GetSingleton();
	void Init(VkPhysicalDeviceMemoryProperties physicalDeviceMemoryPropertiesObj);
	void DeInit();
		
	void AllocateImageMemory(VkImage * imageObj, VkMemoryPropertyFlags userReq, VkDeviceMemory * memoryObj);
    VkMemoryRequirements AllocateBufferMemory(VkBuffer * bufferObj, VkMemoryPropertyFlags userReq, VkDeviceMemory * memoryObj);

};

