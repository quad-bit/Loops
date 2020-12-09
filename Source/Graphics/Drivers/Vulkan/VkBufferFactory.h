#pragma once
#include <vector>
#include <RenderingWrapper.h>
#include <vulkan\vulkan.h>

struct BufferWrapper
{
    VkBufferUsageFlags * bufferType;
    VkBuffer * buffer;
    VkDeviceMemory * bufferMemory;
    uint32_t id;
    void * pGpuMem;
};

class VkBufferFactory
{
private:
    VkBufferFactory(){}
    VkBufferFactory(VkBufferFactory const &) {}
    VkBufferFactory const & operator= (VkBufferFactory const &) {}

    static VkBufferFactory* instance;

    uint32_t idCounter = 0;
    uint32_t GetId();

    std::vector<BufferWrapper> bufferWrapperList;

public:
    void Init();
    void DeInit();
    void Update();
    static VkBufferFactory* GetInstance();
    ~VkBufferFactory();

    uint32_t * CreateBuffer(VkBufferUsageFlags * bufferType, VkMemoryPropertyFlags * memType,
        void * data, size_t dataSize, void * pGpuMem);

    void DestroyBuffer(uint32_t id);
};
