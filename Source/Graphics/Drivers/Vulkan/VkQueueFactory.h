#pragma once
#include <vulkan\vulkan.h>
#include <stdint.h>
#include <vector>
#include <RenderingWrapper.h>

//enum class QUEUE_TYPE
//{
//    GRAPHICS,
//    COMPUTE,
//    TRANSFER,
//    GRAPHICS_COMPUTE
//};

enum class FamilyPermutatation
{
    //SAME_FAMILY,
    //SAME_QUEUE,
    SAME_FAMILY_SAME_QUEUE,
    SAME_FAMILY_UNIQUE_QUEUE,
    UNIQUE_QUEUES

};

typedef PipelineType QueueType;

class QueueWrapper
{
public:
    uint32_t queueFamilyIndex;
    VkQueue * queue;
    QueueType queueType;
};


class VkQueueFactory
{
private:
    VkQueueFactory(){}
    VkQueueFactory(VkQueueFactory const &) {}
    VkQueueFactory const & operator= (VkQueueFactory const &) {}

    static VkQueueFactory* instance;

    std::vector<QueueWrapper> queueWrapperList;
    std::vector<VkQueueFamilyProperties> propertyList;
    std::vector<uint32_t> uniqueIndiciesInFamily;
    const float* priority;
    uint32_t familyIndex = -1;

public:
    void Init();
    void DeInit();
    void Update();
    static VkQueueFactory* GetInstance();
    ~VkQueueFactory();

    //void FindQueue(VkQueueFlagBits * queueFlag, uint32_t count, FamilyPermutatation perm, VkDeviceQueueCreateInfo * creatInfo, uint32_t * indexInFamily);
    VkDeviceQueueCreateInfo FindQueue();
    void InitQueues(uint32_t * indexInFamily, uint32_t count);
    void InitQueues();

    VkQueue * GetQueue(QueueType qType);
    VkQueue * GetQueue(VkQueueFlagBits qType);
    uint32_t GetQueueFamilyIndex(VkQueueFlagBits qType) { return familyIndex; }
};
