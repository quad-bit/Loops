#pragma once

#include <vulkan\vulkan.h>
#include <vector>
#include <RenderingWrapper.h>

enum class CMD_BUF_STATES
{
    INITIAL,
    RECORDING,
    EXECUTABLE,
    PENDING,
    INVALID
};

class VkCommandPoolWrapper
{
public:
    VkCommandPool* pool;
    uint32_t poolId;
};

//class VkCommandBufferWrapper
//{
//public:
//    uint32_t id;
//    VkCommandBuffer * commandBuffer;
//    VkCommandPool * pool;
//};


class VkDrawCommandBuffer;

class VkCommandBufferFactory
{

private:
    VkCommandBufferFactory(){}
    VkCommandBufferFactory(VkCommandBufferFactory const &) {}
    VkCommandBufferFactory const & operator= (VkCommandBufferFactory const &) {}

    static VkCommandBufferFactory* instance;
    static uint32_t poolIdCounter, bufferIdCounter;

    std::vector<VkCommandPoolWrapper> poolList;
    //std::vector<VkCommandBufferWrapper> bufferList;

    std::vector<VkDrawCommandBuffer* > drawCommandBufferList;

    VkCommandBuffer * GetCommandBuffer(uint32_t id);
    VkCommandPool * GetCommandPool(uint32_t poolId);

    uint32_t GetPoolId();
    uint32_t GetBufferId();

    VkQueue * renderQueue, * computeQueue, *transferQueue ;
    uint32_t graphicsQueueFamilyId, computeQueueFamilyId, transferQueueFamilyId;

public:
    void Init();
    void DeInit();
    void Update();
    static VkCommandBufferFactory* GetInstance();
    ~VkCommandBufferFactory();

    void CreateCommandPool(VkCommandPoolCreateFlagBits flags, VkQueueFlagBits queueType, uint32_t& id);
    //void CreateCommandBuffer(uint32_t poolId, VkCommandBufferLevel level, uint32_t count, uint32_t * ids);
    VkDrawCommandBuffer * CreateCommandBuffer(uint32_t poolId, VkCommandBufferLevel level, 
        PipelineType commandBufferType, uint32_t * ids);
    void DestroyCommandPool(uint32_t id);
    void ResetCommandPool(uint32_t id);
    void ResetCommandBuffer(uint32_t poolId, uint32_t id);
    void DestroyCommandBuffer(uint32_t id);
};
