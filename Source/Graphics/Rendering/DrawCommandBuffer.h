#pragma once

#include <stdint.h>

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
    PipelineType* pipelineType;

public:
    void Init(T * apiInterface, CommandBufferLevel* level, uint32_t poolId);
    void DeInit();
    DrawCommandBuffer();
    ~DrawCommandBuffer();
};

#include "RenderingWrapper.h"

template<typename T>
inline void DrawCommandBuffer<T>::Init(T * apiInterface, CommandBufferLevel * level, uint32_t poolId)
{
    this->apiInterface = apiInterface;
    apiInterface->CreateCommandBuffer(poolId, cmdBufferId, level);
    this->poolId = poolId;
    this->commandBufferLevel = level;
}

template<typename T>
inline void DrawCommandBuffer<T>::DeInit()
{
    apiInterface->DestroyCommandBuffer(cmdBufferId);
    delete commandBufferLevel;
}

template<typename T>
inline DrawCommandBuffer<T>::DrawCommandBuffer()
{
}

template<typename T>
inline DrawCommandBuffer<T>::~DrawCommandBuffer()
{
}
