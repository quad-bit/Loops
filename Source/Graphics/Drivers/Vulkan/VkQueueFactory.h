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
    uint32_t queueId;
    uint32_t indexInFamily;
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

    //std::vector<QueueWrapper> queueWrapperList;
    std::vector<QueueWrapper> graphicsQueueWrapperList, computeQueueWrapperList, transferQueueWrapperList;
    std::vector<VkQueueFamilyProperties> propertyList;
    //std::vector<uint32_t> uniqueIndiciesInFamily;
    const float* graphicQueuePriority, *computeQueuePriority, *transferQueuePriority;
    //uint32_t familyIndex = -1;

    uint32_t maxGraphicQueues = 3, maxComputeQueues = 2, maxTransferQueue = 1;
    uint32_t graphicsQueueFamilyIndex = -1, computeQueueFamilyIndex = -1, transferQueueFamilyIndex = -1;
    uint32_t graphicQueueInitCounter = 0, computeQueueInitCounter = 0, transferQueueInitCounter = 0;

    uint32_t queueIdCounter = 0;
    uint32_t GetQueueId() { return queueIdCounter++; }

public:
    void Init();
    void DeInit();
    void Update();
    static VkQueueFactory* GetInstance();
    ~VkQueueFactory();

    //void FindQueue(VkQueueFlagBits * queueFlag, uint32_t count, FamilyPermutatation perm, VkDeviceQueueCreateInfo * creatInfo, uint32_t * indexInFamily);
    //void InitQueues(uint32_t * indexInFamily, uint32_t count);
    std::vector<VkDeviceQueueCreateInfo> FindQueue();
    void InitQueues();

    VkQueue * GetQueue(QueueType qType, uint32_t id);
    VkQueue * GetQueue(VkQueueFlagBits qType, uint32_t id);
    //uint32_t GetQueueFamilyIndex(VkQueueFlagBits qType) { return familyIndex; }

    uint32_t GetQueueFamilyIndex(QueueType qType, uint32_t queueId);
    uint32_t GetQueueFamilyIndex(VkQueueFlagBits qType, uint32_t queueId);
    uint32_t GetGraphicsQueueFamilyIndex() { return graphicsQueueFamilyIndex; }

    void CreateGraphicsQueues(uint32_t * ids, const uint32_t & count);
    void CreateComputeQueues(uint32_t * ids, const uint32_t & count);
    void CreateTransferQueues(uint32_t * ids, const uint32_t & count);

    //void DestroyQueue(QueueType qType, uint32_t queueId);
};
