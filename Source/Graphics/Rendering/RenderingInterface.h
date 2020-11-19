#pragma once
#include <Settings.h>
#include "RenderingWrapper.h"

template <typename T>
class ForwardRendering;

template <typename T>
class DeferredRendering;

class VulkanInterface;
class DxInterface;

template <typename T>
class CommandBufferManager;

template <typename T>
class DrawCommandBuffer;

template <typename T>
class RenderingInterface
{
private:
    ForwardRendering<T> * forwardRenderer;
    DrawCommandBuffer<T> ** drawCommandBufferObj;
    uint32_t graphicCommandPoolId, computeCommandPoolId, guiCommandPoolId;

    uint32_t maxFramesInFlight;

    uint32_t * renderSemaphores, *presentationSemaphores;
    uint32_t * getSwapChainImageFences;

    void BeginRenderLoop();
    void EndRenderLoop();

    T * apiInterface;

public:
    void Init(T * apiInterface);
    void SetupRenderer();
    void DislogeRenderer();
    void DeInit();
};

#include "ForwardInterface.h"
#include "CommandBufferManager.h"

template<typename T>
inline void RenderingInterface<T>::BeginRenderLoop()
{
    //get swapchain image index to used for rendering the current frame.
}

template<typename T>
inline void RenderingInterface<T>::EndRenderLoop()
{
}

template<typename T>
inline void RenderingInterface<T>::Init(T * apiInterface)
{
    forwardRenderer = new ForwardRendering<T>();
    forwardRenderer->Init(apiInterface);
    CommandBufferManager<T>::GetInstance()->Init(apiInterface);
    this->apiInterface = apiInterface;
}

template<typename T>
inline void RenderingInterface<T>::SetupRenderer()
{
    PipelineType pipelineType = PipelineType::GRAPHICS;
    CommandPoolProperty prop = CommandPoolProperty::TRANS_RESET;

    graphicCommandPoolId = CommandBufferManager<T>::GetInstance()->CreateCommandPool(&pipelineType, &prop);
    
    pipelineType = PipelineType::COMPUTE;
    computeCommandPoolId = CommandBufferManager<T>::GetInstance()->CreateCommandPool(&pipelineType, &prop);

    pipelineType = PipelineType::GRAPHICS;
    guiCommandPoolId = CommandBufferManager<T>::GetInstance()->CreateCommandPool(&pipelineType, &prop);
    
    forwardRenderer->SetupRenderer();

    drawCommandBufferObj = new DrawCommandBuffer<T>*[Settings::swapBufferCount];

    for (uint32_t i = 0; i < Settings::swapBufferCount; i++)
    {
        CommandBufferLevel * level = new CommandBufferLevel;
        *level = CommandBufferLevel::PRIMARY;
        drawCommandBufferObj[i] = CommandBufferManager<T>::GetInstance()->CreateDrawCommandBuffer(level, graphicCommandPoolId);
    }

    maxFramesInFlight = Settings::swapBufferCount - 1;  
    renderSemaphores = new uint32_t[maxFramesInFlight];
    presentationSemaphores = new uint32_t[maxFramesInFlight];
    getSwapChainImageFences = new uint32_t[maxFramesInFlight];

    for (uint32_t i = 0; i < maxFramesInFlight; i++)
    {
        renderSemaphores[i] = apiInterface->Create_Semaphore(false);
        presentationSemaphores[i] = apiInterface->Create_Semaphore(false);
        getSwapChainImageFences[i] = apiInterface->CreateFence(true);
    }
}

template<typename T>
inline void RenderingInterface<T>::DislogeRenderer()
{

    for (uint32_t i = 0; i < maxFramesInFlight; i++)
    {
        apiInterface->DestroyFence(getSwapChainImageFences[i]);
        apiInterface->DestroySemaphore(renderSemaphores[i]);
        apiInterface->DestroySemaphore(presentationSemaphores[i]);
    }

    delete[] getSwapChainImageFences;
    delete[] presentationSemaphores;
    delete[] renderSemaphores;

    for (uint32_t i = 0; i < Settings::swapBufferCount; i++)
    {
        CommandBufferManager<T>::GetInstance()->DestroyDrawCommandBuffer(drawCommandBufferObj[i]);
        //delete drawCommandBufferObj[i];
    }

    delete[] drawCommandBufferObj;

    forwardRenderer->DislogeRenderer();

    CommandBufferManager<T>::GetInstance()->DestroyCommandPool(guiCommandPoolId);
    CommandBufferManager<T>::GetInstance()->DestroyCommandPool(computeCommandPoolId);
    CommandBufferManager<T>::GetInstance()->DestroyCommandPool(graphicCommandPoolId);
}

template<typename T>
inline void RenderingInterface<T>::DeInit()
{
    CommandBufferManager<T>::GetInstance()->DeInit();
    delete CommandBufferManager<T>::GetInstance();

    forwardRenderer->DeInit();
    delete forwardRenderer;
}
