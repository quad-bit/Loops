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
    DrawCommandBuffer<T> ** drawCommandBufferList;
    uint32_t graphicCommandPoolId, computeCommandPoolId, guiCommandPoolId;

    uint32_t maxFramesInFlight;

    uint32_t * renderSemaphores, *presentationSemaphores;
    uint32_t * getSwapChainImageFences;

    uint32_t currentFrameIndex{ 0 }, currentSwapchainIndex{ 0 };

    void BeginRenderLoop();
    void EndRenderLoop();

    T * apiInterface;

    DrawCommandBuffer<T> * activeDrawCommandBuffer;

public:
    void Init(T * apiInterface);
    void SetupRenderer();
    void DislogeRenderer();
    void DeInit();
    void Render();
};

#include "ForwardInterface.h"
#include "CommandBufferManager.h"

template<typename T>
inline void RenderingInterface<T>::BeginRenderLoop()
{
    //get swapchain image index to be used for rendering the current frame.

    uint32_t fenceId = getSwapChainImageFences[currentFrameIndex];
    uint32_t semaphoreId = renderSemaphores[currentFrameIndex];

    currentSwapchainIndex = apiInterface->GetAvailableSwapchainIndex(fenceId, semaphoreId);

    activeDrawCommandBuffer = drawCommandBufferList[currentSwapchainIndex];

    CommandBufferManager<T>::GetInstance()->ResetDrawCommandBuffer(activeDrawCommandBuffer);
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

    Settings::clearColorValue[0] = 164.0f / 256.0f; // Red
    Settings::clearColorValue[0] = 30.0f / 256.0f;  // Green
    Settings::clearColorValue[0] = 34.0f / 256.0f;  // Blue
    Settings::clearColorValue[0] = 1.0f;

    Settings::depthClearValue = 1.0f;
    Settings::stencilClearValue = 0.0f;
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

    drawCommandBufferList = new DrawCommandBuffer<T>*[Settings::swapBufferCount];

    for (uint32_t i = 0; i < Settings::swapBufferCount; i++)
    {
        CommandBufferLevel * level = new CommandBufferLevel;
        *level = CommandBufferLevel::PRIMARY;
        drawCommandBufferList[i] = CommandBufferManager<T>::GetInstance()->CreateDrawCommandBuffer(level, graphicCommandPoolId);
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
        CommandBufferManager<T>::GetInstance()->DestroyDrawCommandBuffer(drawCommandBufferList[i]);
        //delete drawCommandBufferObj[i];
    }

    delete[] drawCommandBufferList;

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

template<typename T>
inline void RenderingInterface<T>::Render()
{
    //BeginRenderLoop();

    //EndRenderLoop();
}
