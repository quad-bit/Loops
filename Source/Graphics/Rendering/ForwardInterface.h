#include <CorePrecompiled.h>
#pragma once

#include "VulkanInterface.h"
#include "DxInterface.h"

template <typename T>
class DrawCommandBuffer;

template<typename T>
class ForwardRendering
{
private:
    T * apiInterface;
    std::vector<uint32_t> defaultRenderTargetList, defaultDepthTargetList;
    uint32_t defaultRenderPassId;
    Format bestDepthFormat;
    std::vector<uint32_t> defaultFbos;
    uint32_t depthImageMemoryId;

    bool sampleRateShadingEnabled = false;
    bool multiSamplingAvailable = false;
    Samples desiredSampleCountForMSAA = Samples::SAMPLE_COUNT_8_BIT;
    Samples CheckForMSAA();
    
    Format windowSurfaceFormat;
    ColorSpace windowColorSpace;

    void SetupRendererForMSAA(Samples sampleCount);

public:
    void Init();
    void Init(T * apiInterface);
    void SetupRenderer();
    void DislogeRenderer();
    void PreRender();
    void PostRender();
    void BeginRender(DrawCommandBuffer<T> * drawCommandBuffer, const uint32_t & activeSwapChainIndex);
    void EndRender(DrawCommandBuffer<T> * drawCommandBuffer);
    void DeInit();
};

#include "RenderingWrapper.h"
#include <Settings.h>
#include <CorePrecompiled.h>
#include "GraphicsPipelineManager.h"

template<typename T>
inline Samples ForwardRendering<T>::CheckForMSAA()
{
    Samples sampleCount;
    if (RendererSettings::MSAA_Enabled)
    {
        sampleCount = apiInterface->GetMaxUsableSampleCount();

        ASSERT_MSG_DEBUG(sampleCount != Samples::SAMPLE_COUNT_1_BIT, "MSAA not available");


        if (sampleCount != Samples::SAMPLE_COUNT_1_BIT)
        {
            if (apiInterface->IsSampleRateShadingAvailable())
            {
                sampleRateShadingEnabled = true;
            }

            multiSamplingAvailable = false;

            if (sampleCount < desiredSampleCountForMSAA)
            {

            }
            else if (sampleCount > desiredSampleCountForMSAA)
            {
                sampleCount = desiredSampleCountForMSAA;
            }

        }
        else
        {
            multiSamplingAvailable = false;
        }
    }
    else
    {
        sampleCount = Samples::SAMPLE_COUNT_1_BIT;
    }

    return sampleCount;
}

template<typename T>
inline void ForwardRendering<T>::SetupRendererForMSAA(Samples sampleCount)
{
#if 0
    
    ImageInfo * info = new ImageInfo();
    info->colorSpace = windowColorSpace;
    info->degree = Dimensions::Dim2;
    info->format = windowSurfaceFormat;
    info->height = Settings::windowHeight;
    info->layers = 1;
    info->mips = 1;
    info->sampleCount = sampleCount;
    info->usage = Usage::USAGE_COLOR_ATTACHMENT_BIT;
    info->width = Settings::windowWidth;
    info->initialLayout = ImageLayout::LAYOUT_UNDEFINED;

    apiInterface->CreateRenderTarget(info, Settings::swapBufferCount, true, &defaultRenderTargetList);

    Format * depthFormats = new Format[5];
    depthFormats[0] = Format::D32_SFLOAT_S8_UINT;
    depthFormats[1] = Format::D24_UNORM_S8_UINT;
    depthFormats[2] = Format::D16_UNORM_S8_UINT;
    depthFormats[3] = Format::D32_SFLOAT;
    depthFormats[4] = Format::D16_UNORM;

    uint32_t index = apiInterface->FindBestDepthFormat(depthFormats, 5);

    ASSERT_MSG_DEBUG(index != -1, "depth format not available");

    bestDepthFormat = depthFormats[index];

    delete[] depthFormats;

    //reusing info without new allocation
    info->degree = Dimensions::Dim2;
    info->format = bestDepthFormat;
    info->height = Settings::windowHeight;
    info->layers = 1;
    info->mips = 1;
    info->sampleCount = sampleCount;
    info->usage = Usage::USAGE_DEPTH_STENCIL_ATTACHMENT_BIT;
    info->width = Settings::windowWidth;
    info->initialLayout = ImageLayout::LAYOUT_UNDEFINED;


    //apiInterface->CreateDepthTarget(info, Settings::swapBufferCount, true, true, &defaultDepthTargetList);

    defaultDepthTargetList.resize(Settings::swapBufferCount);
    apiInterface->CreateDepthTarget(info, Settings::swapBufferCount, true, true, defaultDepthTargetList.data());

    delete info;
#endif
}

template<typename T>
inline void ForwardRendering<T>::Init()
{
    PLOGD << "Forward rendering Init";

    apiInterface = new T();
    apiInterface->Init();
}

template<typename T>
inline void ForwardRendering<T>::Init(T * apiInterface)
{
    PLOGD << "Forward rendering Init";

    this->apiInterface = apiInterface;
    apiInterface->Init();
}

template<typename T>
inline void ForwardRendering<T>::SetupRenderer()
{
    Samples sampleCount = CheckForMSAA();
    windowSurfaceFormat = apiInterface->GetWindowSurfaceFormat();
    windowColorSpace = apiInterface->GetWindowColorSpace();
    
    // swapchain / presentation setup
    {
        ImageInfo info = {};
        info.colorSpace = windowColorSpace;
        info.format = windowSurfaceFormat;
        info.width = Settings::windowWidth;
        info.height = Settings::windowHeight;
        info.imageType = ImageType::IMAGE_TYPE_2D;
        info.usage = Usage::USAGE_COLOR_ATTACHMENT_BIT;

        apiInterface->SetupPresentationEngine(info);
    }
    
    if (RendererSettings::MSAA_Enabled && multiSamplingAvailable)
    {
        SetupRendererForMSAA(sampleCount);
    }
    else
    {
        // default render target setup
        {
            ImageInfo info;
            info.colorSpace = windowColorSpace;
            info.imageType = ImageType::IMAGE_TYPE_2D;
            info.format = windowSurfaceFormat;
            info.layers = 1;
            info.mips = 1;
            info.sampleCount = sampleCount;
            info.usage = Usage::USAGE_COLOR_ATTACHMENT_BIT;
            info.height = Settings::windowHeight;
            info.width = Settings::windowWidth;
            info.depth = 1;
            info.initialLayout = ImageLayout::LAYOUT_UNDEFINED;

            ImageViewInfo viewInfo = {};
            viewInfo.baseArrayLayer = 0;
            viewInfo.baseMipLevel = 0;
            viewInfo.components[0] = ComponentSwizzle::COMPONENT_SWIZZLE_IDENTITY;
            viewInfo.components[1] = ComponentSwizzle::COMPONENT_SWIZZLE_IDENTITY;
            viewInfo.components[2] = ComponentSwizzle::COMPONENT_SWIZZLE_IDENTITY;
            viewInfo.components[3] = ComponentSwizzle::COMPONENT_SWIZZLE_IDENTITY;
            viewInfo.format = windowSurfaceFormat;
            viewInfo.imageAspect = ImageAspectFlag::IMAGE_ASPECT_COLOR_BIT;
            viewInfo.layerCount = 1;
            viewInfo.levelCount = 1;
            viewInfo.viewType = ImageViewType::IMAGE_VIEW_TYPE_2D;

            defaultRenderTargetList.resize(Settings::swapBufferCount);
            //apiInterface->CreateRenderTarget(info, Settings::swapBufferCount, true, &defaultRenderTargetList);
            apiInterface->CreateDefaultRenderTarget(info, viewInfo, Settings::swapBufferCount, defaultRenderTargetList.data());
        }

        // depth
        {
            Format * depthFormats = new Format[5];
            depthFormats[0] = Format::D32_SFLOAT_S8_UINT;
            depthFormats[1] = Format::D24_UNORM_S8_UINT;
            depthFormats[2] = Format::D16_UNORM_S8_UINT;
            depthFormats[3] = Format::D32_SFLOAT;
            depthFormats[4] = Format::D16_UNORM;

            uint32_t index = apiInterface->FindBestDepthFormat(depthFormats, 5);

            ASSERT_MSG_DEBUG(index != -1, "depth format not available");
            bestDepthFormat = depthFormats[index];
            delete[] depthFormats;

            ImageInfo info;
            info.imageType = ImageType::IMAGE_TYPE_2D;
            info.format = bestDepthFormat;
            info.layers = 1;
            info.mips = 1;
            info.sampleCount = sampleCount;
            info.usage = Usage::USAGE_DEPTH_STENCIL_ATTACHMENT_BIT;
            info.width = Settings::windowWidth;
            info.height = Settings::windowHeight;
            info.depth = 1;
            info.initialLayout = ImageLayout::LAYOUT_UNDEFINED;
            bool stencilRequired = false;
            bool stencilAvailable = false;

            if (stencilRequired)
                if (info.format == Format::D32_SFLOAT_S8_UINT ||
                    info.format == Format::D24_UNORM_S8_UINT ||
                    info.format == Format::D16_UNORM_S8_UINT)
                    stencilAvailable = true;

            
            defaultDepthTargetList.resize(Settings::swapBufferCount);
            apiInterface->CreateDepthTarget(&info, Settings::swapBufferCount, defaultDepthTargetList.data());
            
            // Get the image memory req
            MemoryRequirementInfo req = apiInterface->GetImageMemoryRequirement(defaultDepthTargetList[0]);

            // Allocate the memory
            size_t allocationSize = req.size * Settings::swapBufferCount;
            MemoryType userReq[1]{ MemoryType::DEVICE_LOCAL_BIT };
            depthImageMemoryId = apiInterface->AllocateMemory(&req, &userReq[0], allocationSize);

            // Bind the memory to the image
            for(uint32_t i = 0; i < defaultDepthTargetList.size(); i++)
                apiInterface->BindImageMemory(defaultDepthTargetList[i], depthImageMemoryId, req.size * i);

            // Create image View
            ImageViewInfo viewInfo = {};
            viewInfo.baseArrayLayer = 0;
            viewInfo.baseMipLevel = 0;
            viewInfo.components[0] = ComponentSwizzle::COMPONENT_SWIZZLE_IDENTITY;
            viewInfo.components[1] = ComponentSwizzle::COMPONENT_SWIZZLE_IDENTITY;
            viewInfo.components[2] = ComponentSwizzle::COMPONENT_SWIZZLE_IDENTITY;
            viewInfo.components[3] = ComponentSwizzle::COMPONENT_SWIZZLE_IDENTITY;
            viewInfo.format = bestDepthFormat;
            // TODO : need to fix this OR thing
            viewInfo.imageAspect = ImageAspectFlag::IMAGE_ASPECT_DEPTH_BIT;// | (stencilAvailable ? IMAGE_ASPECT_STENCIL_BIT : 0);
            viewInfo.layerCount = 1;
            viewInfo.levelCount = 1;
            viewInfo.viewType = ImageViewType::IMAGE_VIEW_TYPE_2D;

            std::vector<ImageViewInfo> viewInfoList{ viewInfo, viewInfo, viewInfo};

            for (uint32_t i = 0; i < defaultDepthTargetList.size(); i++)
            {
                viewInfoList[i].imageId = defaultDepthTargetList[i];
            }
            uint32_t count = (uint32_t)viewInfoList.size();
            apiInterface->CreateImageView(viewInfoList.data(), count);
        }

        RenderPassAttachmentInfo attchmentDescList[2];

        attchmentDescList[0].finalLayout = ImageLayout::LAYOUT_PRESENT_SRC_KHR;
        attchmentDescList[0].format = Format::B8G8R8A8_UNORM;
        attchmentDescList[0].initialLayout = ImageLayout::LAYOUT_UNDEFINED;
        attchmentDescList[0].loadOp = LoadOperation::LOAD_OP_CLEAR;
        attchmentDescList[0].sampleCount = sampleCount;
        attchmentDescList[0].stencilLoadOp = LoadOperation::LOAD_OP_DONT_CARE;
        attchmentDescList[0].stencilLStoreOp = StoreOperation::STORE_OP_DONT_CARE;
        attchmentDescList[0].storeOp = StoreOperation::STORE_OP_STORE;

        attchmentDescList[1].finalLayout = ImageLayout::LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
        attchmentDescList[1].format = bestDepthFormat;
        attchmentDescList[1].initialLayout = ImageLayout::LAYOUT_UNDEFINED;
        attchmentDescList[1].loadOp = LoadOperation::LOAD_OP_CLEAR;
        attchmentDescList[1].sampleCount = sampleCount;
        attchmentDescList[1].stencilLoadOp = LoadOperation::LOAD_OP_DONT_CARE;
        attchmentDescList[1].stencilLStoreOp = StoreOperation::STORE_OP_DONT_CARE;
        attchmentDescList[1].storeOp = StoreOperation::STORE_OP_DONT_CARE;

        AttachmentRef colorAttachmentRef;
        colorAttachmentRef.index = 0;
        colorAttachmentRef.layoutInSubPass = ImageLayout::LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

        AttachmentRef depthAttachmentRef;
        depthAttachmentRef.index = 1;
        depthAttachmentRef.layoutInSubPass = ImageLayout::LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

        SubpassInfo subpassInfo;
        subpassInfo.colorAttachmentCount = 1;
        subpassInfo.inputAttachmentCount = 0;
        subpassInfo.pColorAttachments = &colorAttachmentRef;
        subpassInfo.pDepthStencilAttachment = &depthAttachmentRef;
        subpassInfo.pInputAttachments = nullptr;
        subpassInfo.pResolveAttachments = nullptr;

        apiInterface->CreateRenderPass(
            attchmentDescList, 2,
            &subpassInfo, 1,
            nullptr, 0,
            defaultRenderPassId
        );

        uint32_t imagesPerFbo = 2, numFbos = Settings::swapBufferCount;
        uint32_t * imageIds = new uint32_t[numFbos * imagesPerFbo];

        for (uint32_t i = 0, j = 0; i < numFbos * imagesPerFbo; i++, j++)
        {
            imageIds[i++] = defaultRenderTargetList[j];
            imageIds[i] = defaultDepthTargetList[j];
        }

        defaultFbos.resize(numFbos);
        apiInterface->CreateFrameBuffer(numFbos, imageIds, imagesPerFbo, defaultRenderPassId,
            Settings::windowWidth, Settings::windowHeight, &defaultFbos[0]);

        delete[] imageIds;
    }
}

template<typename T>
inline void ForwardRendering<T>::DislogeRenderer()
{
    apiInterface->DestroyFrameBuffer(defaultFbos.data(), (uint32_t)defaultFbos.size());
    apiInterface->DestroyRenderPass(defaultRenderPassId);
    //apiInterface->DestroyDepthTarget(&defaultDepthTargetList, true);
    //apiInterface->DestroyRenderTarget(&defaultRenderTargetList, true);

    bool viewDestroyList[3]{ true, true, true };
    bool freeMemoryList[3]{ false, false, false };
    apiInterface->DestroyAttachment(defaultDepthTargetList.data(), viewDestroyList, freeMemoryList, (uint32_t)defaultDepthTargetList.size());
    apiInterface->FreeAttachmentMemory(&defaultDepthTargetList[0], 1);
    apiInterface->DestroySwapChainImageViews(defaultRenderTargetList.data(), (uint32_t)defaultRenderTargetList.size());
}

template<typename T>
inline void ForwardRendering<T>::PreRender()
{
    GraphicsPipelineManager<T>::GetInstance()->GenerateAllPipelines(defaultRenderPassId, 0);
}

template<typename T>
inline void ForwardRendering<T>::PostRender()
{
}

template<typename T>
inline void ForwardRendering<T>::BeginRender(DrawCommandBuffer<T>* drawCommandBuffer, const uint32_t & activeSwapChainIndex)
{
    //set the begin info for the render pass
    
    RenderPassBeginInfo info = {};
    info.frameBufferId = defaultFbos[activeSwapChainIndex];
    
    info.clearColorValue[0] = Settings::clearColorValue[0];
    info.clearColorValue[1] = Settings::clearColorValue[1];
    info.clearColorValue[2] = Settings::clearColorValue[2];
    info.clearColorValue[3] = Settings::clearColorValue[3];

    info.depthClearValue = Settings::depthClearValue;
    info.stencilClearValue = Settings::stencilClearValue;
    info.renderPassId = defaultRenderPassId;
    info.renderAreaExtent[0] = (float)Settings::windowWidth;
    info.renderAreaExtent[1] = (float)Settings::windowHeight;
    info.renderAreaPosition[0] = 0.0f;
    info.renderAreaPosition[1] = 0.0f;

    SubpassContentStatus subpassStatus = SubpassContentStatus::SUBPASS_CONTENTS_INLINE;

    drawCommandBuffer->BeginRenderPass(&info, &subpassStatus);

    Viewport viewport;
    viewport.height = (float)Settings::windowHeight;
    viewport.width = (float)Settings::windowWidth;
    viewport.minDepth = (float)0.0f;
    viewport.maxDepth = (float)1.0f;
    viewport.x = 0;
    viewport.y = 0;
    //vkCmdSetViewport(CommandBufferManager::GetSingleton()->GetCommandBufferObj(), 0, 1, &viewport);
    drawCommandBuffer->SetViewport(viewport.width, viewport.height, viewport.x, viewport.y, viewport.minDepth, viewport.maxDepth);

    Rect2D scissor;
    scissor.lengthX = (float)Settings::windowWidth;
    scissor.lengthY = (float)Settings::windowHeight;
    scissor.offsetX = 0;
    scissor.offsetY = 0;
    //vkCmdSetScissor(CommandBufferManager::GetSingleton()->GetCommandBufferObj(), 0, 1, &scissor);
    drawCommandBuffer->SetScissor(scissor.lengthX, scissor.lengthY, scissor.offsetX, scissor.offsetY);
}

template<typename T>
inline void ForwardRendering<T>::EndRender(DrawCommandBuffer<T>* drawCommandBuffer)
{
    drawCommandBuffer->EndRenderPass();
}

template<typename T>
inline void ForwardRendering<T>::DeInit()
{
    PLOGD << "Forward rendering deinit";

    apiInterface->DeInit();
}
