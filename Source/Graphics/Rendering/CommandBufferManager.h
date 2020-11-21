#pragma once
#include <Settings.h>
#include <Assertion.h>
#include <vector>

enum class CommandPoolProperty;
enum class PipelineType;
enum class CommandBufferLevel;
typedef PipelineType CommandBufferType;

template <typename T>
class DrawCommandBuffer;

class VulkanInterface;

#define MAX_COMMANDBUFFERS 10

struct PoolWrapper
{
    uint32_t poolId;
    CommandPoolProperty * prop;
    PipelineType * pipelineType;
};


template < typename T>
class CommandBufferManager
{
private:
    CommandBufferManager() {};
    CommandBufferManager(CommandBufferManager const &) {}
    CommandBufferManager const & operator= (CommandBufferManager const &) {}

    static CommandBufferManager* instance;

    T * apiInterface;
    uint32_t idCounter = 0, drawCB_Counter = 0;

    DrawCommandBuffer<T> * drawCommandBufferList;
    //DispatchCommandBuffer<T> * DispatchCommandBufferList;
    //TransferCommandBuffer<T> * transferCommandBufferList;


public:
    static CommandBufferManager* GetInstance();
    ~CommandBufferManager();
    void Init(T * apiInterface);
    void DeInit();
    void Update();
    DrawCommandBuffer<T> * CreateDrawCommandBuffer(CommandBufferLevel* type, uint32_t poolId);
    void DestroyCommandBuffer(uint32_t commandBufferId);
    void DestroyDrawCommandBuffer(DrawCommandBuffer<T> * obj);
    void ResetCommandBuffer(uint32_t commandBufferId);
    void ResetDrawCommandBuffer(DrawCommandBuffer<T> * obj);
    uint32_t CreateCommandPool(PipelineType * poolType, CommandPoolProperty * poolProp);
    void DestroyCommandPool(uint32_t commandPoolId);
};

#include "RenderingWrapper.h"
#include "DrawCommandBuffer.h"

template<typename T>
inline void CommandBufferManager<T>::Init(T * apiInterface)
{
    drawCommandBufferList = new DrawCommandBuffer<T>[MAX_COMMANDBUFFERS];
    //DispatchCommandBufferList; = new DispatchCommandBuffer<T>[MAX_COMMANDBUFFERS];
    //transferCommandBuffer = new TransferCommandBuffer<T>;
    this->apiInterface = apiInterface;
}

template<typename T>
inline void CommandBufferManager<T>::DeInit()
{
    delete[] drawCommandBufferList;
}

template<typename T>
inline void CommandBufferManager<T>::Update()
{
}

template<typename T>
inline DrawCommandBuffer<T> * CommandBufferManager<T>::CreateDrawCommandBuffer(CommandBufferLevel* level, uint32_t poolId)
{
    //create draw command buffer
    ASSERT_MSG(drawCB_Counter <= MAX_COMMANDBUFFERS, "Command buffers exhausted");
    
    drawCommandBufferList[drawCB_Counter].Init(apiInterface, level, poolId);
    drawCB_Counter;

    return &drawCommandBufferList[drawCB_Counter++];
}

template<typename T>
inline void CommandBufferManager<T>::DestroyCommandBuffer(uint32_t commandBufferId)
{

    //drawCommandBufferList[drawCB_Counter].Init(apiInterface, level);
}

template<typename T>
inline void CommandBufferManager<T>::DestroyDrawCommandBuffer(DrawCommandBuffer<T>* obj)
{
    obj->DeInit();
}

template<typename T>
inline void CommandBufferManager<T>::ResetCommandBuffer(uint32_t commandBufferId)
{
}

template<typename T>
inline uint32_t CommandBufferManager<T>::CreateCommandPool(PipelineType * poolType, CommandPoolProperty * poolProp)
{
    return apiInterface->CreateCommandPool(poolType, poolProp);
}

template<typename T>
inline void CommandBufferManager<T>::DestroyCommandPool(uint32_t commandPoolId)
{
    apiInterface->DestroyCommandPool(commandPoolId);
}

template<typename T>
inline CommandBufferManager<T>::~CommandBufferManager()
{
}
