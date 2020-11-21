#pragma once
#include <stdint.h>

#if (RENDERING_API == VULKAN)
class VkDrawCommandBuffer;
#elif (RENDERING_API == DX)
apiInterface = new DxInterface();
#endif

enum class CommandBufferLevel;
enum class PipelineType;

template <typename T>
class DrawCommandBuffer
{
private:
    T * apiInterface;
    uint32_t poolId;
    uint32_t queueId;
    uint32_t cmdBufferId;
    CommandBufferLevel* commandBufferLevel;
    PipelineType* bufferType;

#if (RENDERING_API == VULKAN)
    VkDrawCommandBuffer * commandInterface;
#elif (RENDERING_API == DX)
    
#endif

public:
    void Init(T * apiInterface, CommandBufferLevel* level, const uint32_t & poolId);
    void DeInit();
    DrawCommandBuffer();
    ~DrawCommandBuffer();

#if (RENDERING_API == VULKAN)
    void SetCommandInterface(VkDrawCommandBuffer * commandInterface);
#elif (RENDERING_API == DX)

#endif
    const uint32_t & GetId() { return cmdBufferId; }
    const uint32_t & GetPoolId() { return poolId; }

    void SetViewport(uint32_t commandBufferId, float width, float height, float positionX, float positionY);
    void SetScissor(uint32_t commandBufferId, float width, float height, float positionX, float positionY);
    //vkCmdBeginRenderPass(CommandBufferManager::GetSingleton()->GetCommandBufferObj(), &renderPassBeginInfo, VK_SUBPASS_CONTENTS_INLINE);
    //void BeginRenderPass(uint32_t commandBufferId, RenderPassBeginInfo * renderPassBeginInfo )
};

#include "RenderingWrapper.h"

#if (RENDERING_API == VULKAN)
#include <VkDrawCommandBuffer.h>
#elif (RENDERING_API == DX)
apiInterface = new DxInterface();
#endif

template<typename T>
inline void DrawCommandBuffer<T>::Init(T * apiInterface, CommandBufferLevel * level, const uint32_t & poolId)
{
    this->apiInterface = apiInterface;

    PipelineType * bufType = new PipelineType;
    this->bufferType = bufType;

#if (RENDERING_API == VULKAN)
    commandInterface = apiInterface->CreateCommandBuffer(poolId, &cmdBufferId, level, PipelineType::GRAPHICS);
#elif (RENDERING_API == DX)
    apiInterface = new DxInterface();
#endif
    
    this->poolId = poolId;
    this->commandBufferLevel = level;
}

template<typename T>
inline void DrawCommandBuffer<T>::DeInit()
{
    apiInterface->DestroyCommandBuffer(cmdBufferId);
    delete commandBufferLevel;
    delete bufferType;
}

template<typename T>
inline DrawCommandBuffer<T>::DrawCommandBuffer()
{
}

template<typename T>
inline DrawCommandBuffer<T>::~DrawCommandBuffer()
{
}

template<typename T>
inline void DrawCommandBuffer<T>::SetCommandInterface(VkDrawCommandBuffer * commandInterface)
{
}

template<typename T>
inline void DrawCommandBuffer<T>::SetViewport(uint32_t commandBufferId, float width, float height, float positionX, float positionY)
{
}

template<typename T>
inline void DrawCommandBuffer<T>::SetScissor(uint32_t commandBufferId, float width, float height, float positionX, float positionY)
{
}
