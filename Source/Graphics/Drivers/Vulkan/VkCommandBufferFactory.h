#pragma once

#include <vulkan\vulkan.h>
#include <vector>

enum class CMD_BUF_STATES
{
    INITIAL,
    RECORDING,
    EXECUTABLE,
    PENDING,
    INVALID
};

class CommandPoolWrapper
{
public:
    VkCommandPool* pool;
    uint32_t poolId;
};

class CommandBufferWrapper
{
public:
    uint32_t id;
    VkCommandBuffer * commandBuffer;
    VkCommandPool * pool;
};

class VkCommandBufferFactory
{

private:
    VkCommandBufferFactory(){}
    VkCommandBufferFactory(VkCommandBufferFactory const &) {}
    VkCommandBufferFactory const & operator= (VkCommandBufferFactory const &) {}

    static VkCommandBufferFactory* instance;
    static uint32_t poolIdCounter, bufferIdCounter;

    std::vector<CommandPoolWrapper> poolList;
    std::vector<CommandBufferWrapper> bufferList;

    VkCommandBuffer * GetCommandBuffer(uint32_t id);
    VkCommandPool * GetCommandPool(uint32_t poolId);

    uint32_t GetPoolId();
    uint32_t GetBufferId();

    VkQueue * graphicsQueue, * computeQueue, *transferQueue ;
    uint32_t graphicsQueueFamilyId, computeQueueFamilyId, transferQueueFamilyId;

public:
    void Init();
    void DeInit();
    void Update();
    static VkCommandBufferFactory* GetInstance();
    ~VkCommandBufferFactory();

    void CreateCommandPool(VkCommandPoolCreateFlagBits flags, VkQueueFlagBits queueType, uint32_t& id);
    void CreateCommandBuffer(uint32_t poolId, VkCommandBufferLevel level, uint32_t count, uint32_t * ids);
    void DestroyCommandPool(uint32_t id);
    void ResetCommandPool(uint32_t id);
    void ResetCommandBuffer(uint32_t poolId, uint32_t id);
    void DestroyCommandBuffer(uint32_t id);
};
