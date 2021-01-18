#pragma once
#include <vector>
#include <RenderingWrapper.h>
#include <vulkan\vulkan.h>

struct VkBufferWrapper
{
    uint32_t id;
    VkBufferUsageFlags bufferType;
    VkBuffer * buffer;
    VkDeviceMemory * bufferMemory;
    bool isBufferSharingMemory;
    VkDeviceSize memoryOffset;
    VkMemoryRequirements bufMemReq;
    VkMemoryPropertyFlags memFlags;
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

    std::vector<VkBufferWrapper> bufferWrapperList;

public:
    void Init();
    void DeInit();
    void Update();
    static VkBufferFactory* GetInstance();
    ~VkBufferFactory();

   /* uint32_t * CreateBuffer(VkBufferUsageFlags * bufferType, VkMemoryPropertyFlags * memType,
        void * data, size_t dataSize, void * pGpuMem);
*/
    uint32_t * CreateBuffers(const uint32_t & bufferCount, VkBufferUsageFlags * bufferType, VkMemoryPropertyFlags * memType, size_t * dataSizes);
    uint32_t * AllocateBufferMemory(uint32_t * bufferId, const uint32_t & bufCount);
    uint32_t AllocateSharedMemory(uint32_t * bufferId, const uint32_t & bufCount);

    void CopyBufferDataToMemory(const uint32_t & bufId, VkDeviceSize dataSize, void * data, VkDeviceSize memoryOffset, bool keepMemoryMounted = false);
    void DestroyBuffer(uint32_t id);
};
