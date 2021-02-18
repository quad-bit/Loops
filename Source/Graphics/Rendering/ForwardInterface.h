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
    std::vector<uint32_t> msaaRenderTargetList, msaaDepthTargetList;
    uint32_t defaultRenderPassId;
    Format bestDepthFormat;
    std::vector<uint32_t> defaultFbos;
    uint32_t depthImageMemoryId;
    uint32_t msaaColorMemId, msaaDepthMemId;

    /*bool sampleRateShadingEnabled = false;
    bool multiSamplingAvailable = false;
    Samples CheckForMSAA();
    */
    Format windowSurfaceFormat;
    ColorSpace windowColorSpace;

    void SetupAttachmentsForMSAA(Samples sampleCount);
    //void SetupFrameBufferForMSAA();

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

/*
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

            multiSamplingAvailable = true;

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
*/
template<typename T>
inline void ForwardRendering<T>::SetupAttachmentsForMSAA(Samples sampleCount)
{
    // multi sampled image setup
    {
        ImageInfo info;
        info.colorSpace = windowColorSpace;
        info.imageType = ImageType::IMAGE_TYPE_2D;
        info.format = windowSurfaceFormat;
        info.layers = 1;
        info.mips = 1;
        info.sampleCount = *RendererSettings::sampleCount;
        info.usage = Usage::USAGE_COLOR_ATTACHMENT_BIT;
        info.height = Settings::windowHeight;
        info.width = Settings::windowWidth;
        info.depth = 1;
        info.initialLayout = ImageLayout::LAYOUT_UNDEFINED;

        msaaRenderTargetList.resize(Settings::swapBufferCount);
        apiInterface->CreateAttachment(&info, Settings::swapBufferCount, msaaRenderTargetList.data());
        
        //memory
        {
            // Get the image memory req
            MemoryRequirementInfo req = apiInterface->GetImageMemoryRequirement(msaaRenderTargetList[0]);

            // Allocate the memory
            size_t allocationSize = req.size * Settings::swapBufferCount;
            MemoryType userReq[1]{ MemoryType::DEVICE_LOCAL_BIT };
            msaaColorMemId = apiInterface->AllocateMemory(&req, &userReq[0], allocationSize);

            // Bind the memory to the image
            for (uint32_t i = 0; i < msaaRenderTargetList.size(); i++)
                apiInterface->BindImageMemory(msaaRenderTargetList[i], msaaColorMemId, req.size * i);

        }

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

        std::vector<ImageViewInfo> viewInfoList(Settings::swapBufferCount, viewInfo);// , viewInfo, viewInfo };

        for (uint32_t i = 0; i < msaaRenderTargetList.size(); i++)
        {
            viewInfoList[i].imageId = msaaRenderTargetList[i];
        }
        uint32_t count = (uint32_t)viewInfoList.size();
        apiInterface->CreateImageView(viewInfoList.data(), count);
    }

    // depth
    {
        ImageInfo info;
        info.imageType = ImageType::IMAGE_TYPE_2D;
        info.format = bestDepthFormat;
        info.layers = 1;
        info.mips = 1;
        info.sampleCount = *RendererSettings::sampleCount;
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

        msaaDepthTargetList.resize(Settings::swapBufferCount);
        apiInterface->CreateAttachment(&info, Settings::swapBufferCount, msaaDepthTargetList.data());
        
        {
            // Get the image memory req
            MemoryRequirementInfo req = apiInterface->GetImageMemoryRequirement(msaaDepthTargetList[0]);

            // Allocate the memory
            size_t allocationSize = req.size * Settings::swapBufferCount;
            MemoryType userReq[1]{ MemoryType::DEVICE_LOCAL_BIT };
            msaaDepthMemId = apiInterface->AllocateMemory(&req, &userReq[0], allocationSize);

            // Bind the memory to the image
            for (uint32_t i = 0; i < msaaDepthTargetList.size(); i++)
                apiInterface->BindImageMemory(msaaDepthTargetList[i], msaaDepthMemId, req.size * i);
        }

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

        std::vector<ImageViewInfo> viewInfoList(Settings::swapBufferCount, viewInfo);// , viewInfo, viewInfo };
        for (uint32_t i = 0; i < msaaDepthTargetList.size(); i++)
        {
            viewInfoList[i].imageId = msaaDepthTargetList[i];
        }
        uint32_t count = (uint32_t)viewInfoList.size();
        apiInterface->CreateImageView(viewInfoList.data(), count);
    }

    //render pass
    {
        uint32_t msaaRenderTargetIndex = 0;
        uint32_t defaultRenderTargetIndex = 1;
        uint32_t msaaDepthTargetIndex = 2;
        uint32_t defaultDepthTargetIndex = 3;

        RenderPassAttachmentInfo attchmentDescList[4];

        attchmentDescList[msaaRenderTargetIndex].format = windowSurfaceFormat;
        attchmentDescList[msaaRenderTargetIndex].initialLayout = ImageLayout::LAYOUT_UNDEFINED;
        attchmentDescList[msaaRenderTargetIndex].finalLayout = ImageLayout::LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
        attchmentDescList[msaaRenderTargetIndex].loadOp = LoadOperation::LOAD_OP_CLEAR;
        attchmentDescList[msaaRenderTargetIndex].storeOp = StoreOperation::STORE_OP_STORE;
        attchmentDescList[msaaRenderTargetIndex].sampleCount = *RendererSettings::sampleCount;
        attchmentDescList[msaaRenderTargetIndex].stencilLoadOp = LoadOperation::LOAD_OP_DONT_CARE;
        attchmentDescList[msaaRenderTargetIndex].stencilLStoreOp = StoreOperation::STORE_OP_DONT_CARE;

        attchmentDescList[defaultRenderTargetIndex].format = windowSurfaceFormat;
        attchmentDescList[defaultRenderTargetIndex].finalLayout = ImageLayout::LAYOUT_PRESENT_SRC_KHR;
        attchmentDescList[defaultRenderTargetIndex].initialLayout = ImageLayout::LAYOUT_UNDEFINED;
        attchmentDescList[defaultRenderTargetIndex].sampleCount = Samples::SAMPLE_COUNT_1_BIT;
        attchmentDescList[defaultRenderTargetIndex].stencilLoadOp = LoadOperation::LOAD_OP_DONT_CARE;
        attchmentDescList[defaultRenderTargetIndex].stencilLStoreOp = StoreOperation::STORE_OP_DONT_CARE;
        attchmentDescList[defaultRenderTargetIndex].loadOp = LoadOperation::LOAD_OP_DONT_CARE;
        attchmentDescList[defaultRenderTargetIndex].storeOp = StoreOperation::STORE_OP_STORE;

        attchmentDescList[msaaDepthTargetIndex].format = bestDepthFormat;
        attchmentDescList[msaaDepthTargetIndex].finalLayout = ImageLayout::LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
        attchmentDescList[msaaDepthTargetIndex].initialLayout = ImageLayout::LAYOUT_UNDEFINED;
        attchmentDescList[msaaDepthTargetIndex].sampleCount = *RendererSettings::sampleCount;
        attchmentDescList[msaaDepthTargetIndex].stencilLoadOp = LoadOperation::LOAD_OP_DONT_CARE;
        attchmentDescList[msaaDepthTargetIndex].stencilLStoreOp = StoreOperation::STORE_OP_DONT_CARE;
        attchmentDescList[msaaDepthTargetIndex].loadOp = LoadOperation::LOAD_OP_CLEAR;
        attchmentDescList[msaaDepthTargetIndex].storeOp = StoreOperation::STORE_OP_STORE;

        attchmentDescList[defaultDepthTargetIndex].format = bestDepthFormat;
        attchmentDescList[defaultDepthTargetIndex].finalLayout = ImageLayout::LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
        attchmentDescList[defaultDepthTargetIndex].initialLayout = ImageLayout::LAYOUT_UNDEFINED;
        attchmentDescList[defaultDepthTargetIndex].sampleCount = Samples::SAMPLE_COUNT_1_BIT;
        attchmentDescList[defaultDepthTargetIndex].stencilLoadOp = LoadOperation::LOAD_OP_DONT_CARE;
        attchmentDescList[defaultDepthTargetIndex].stencilLStoreOp = StoreOperation::STORE_OP_DONT_CARE;
        attchmentDescList[defaultDepthTargetIndex].loadOp = LoadOperation::LOAD_OP_DONT_CARE;
        attchmentDescList[defaultDepthTargetIndex].storeOp = StoreOperation::STORE_OP_STORE;

        AttachmentRef colorAttachmentRef;
        colorAttachmentRef.index = msaaRenderTargetIndex;
        colorAttachmentRef.layoutInSubPass = ImageLayout::LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

        AttachmentRef resolveAttachmentRef;
        resolveAttachmentRef.index = defaultRenderTargetIndex;
        resolveAttachmentRef.layoutInSubPass = ImageLayout::LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

        AttachmentRef depthAttachmentRef;
        depthAttachmentRef.index = msaaDepthTargetIndex;
        depthAttachmentRef.layoutInSubPass = ImageLayout::LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

        SubpassDependency dependency[2];
        dependency[0].srcSubpass = -1;
        dependency[0].dstSubpass = 0;
        dependency[0].srcStageMask.push_back(PipelineStage::BOTTOM_OF_PIPE_BIT);
        dependency[0].dstStageMask.push_back(PipelineStage::COLOR_ATTACHMENT_OUTPUT_BIT);
        dependency[0].srcAccessMask.push_back(AccessFlagBits::ACCESS_MEMORY_READ_BIT);
        dependency[0].dstAccessMask.push_back(AccessFlagBits::ACCESS_COLOR_ATTACHMENT_READ_BIT);
        dependency[0].dstAccessMask.push_back(AccessFlagBits::ACCESS_COLOR_ATTACHMENT_WRITE_BIT);
        dependency[0].dependencyFlags.push_back(DependencyFlagBits::DEPENDENCY_BY_REGION_BIT);
        
        dependency[1].srcSubpass = 0;
        dependency[1].dstSubpass = -1;
        dependency[1].srcStageMask.push_back(PipelineStage::COLOR_ATTACHMENT_OUTPUT_BIT);
        dependency[1].dstStageMask.push_back(PipelineStage::BOTTOM_OF_PIPE_BIT);
        dependency[1].srcAccessMask.push_back(AccessFlagBits::ACCESS_COLOR_ATTACHMENT_READ_BIT);
        dependency[1].srcAccessMask.push_back(AccessFlagBits::ACCESS_COLOR_ATTACHMENT_WRITE_BIT);
        dependency[1].dstAccessMask.push_back(AccessFlagBits::ACCESS_MEMORY_READ_BIT);
        dependency[1].dependencyFlags.push_back(DependencyFlagBits::DEPENDENCY_BY_REGION_BIT);

        SubpassInfo subpassInfo = {};
        subpassInfo.colorAttachmentCount = 1;
        subpassInfo.inputAttachmentCount = 0;
        subpassInfo.pColorAttachments = &colorAttachmentRef;
        subpassInfo.pDepthStencilAttachment = &depthAttachmentRef;
        subpassInfo.pInputAttachments = nullptr;
        subpassInfo.pResolveAttachments = &resolveAttachmentRef;
        
        apiInterface->CreateRenderPass(
            attchmentDescList, 4,
            &subpassInfo, 1,
            dependency, 2,
            defaultRenderPassId
        );
    }
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
    //Samples sampleCount = CheckForMSAA();
    windowSurfaceFormat = apiInterface->GetWindowSurfaceFormat();
    windowColorSpace = apiInterface->GetWindowColorSpace();
    
    {
        Format * depthFormats = new Format[5];
        depthFormats[0] = Format::D32_SFLOAT_S8_UINT;
        depthFormats[1] = Format::D24_UNORM_S8_UINT;
        depthFormats[2] = Format::D16_UNORM_S8_UINT;
        depthFormats[3] = Format::D32_SFLOAT;
        depthFormats[4] = Format::D16_UNORM;

        int index = apiInterface->FindBestDepthFormat(depthFormats, 5);

        ASSERT_MSG_DEBUG(index != -1, "depth format not available");
        bestDepthFormat = depthFormats[index];
        delete[] depthFormats;
    }

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
    
    //if (RendererSettings::MSAA_Enabled && multiSamplingAvailable)
    //{
    //}
    //else
    {
        // default render target setup
        {
            ImageInfo info;
            info.colorSpace = windowColorSpace;
            info.imageType = ImageType::IMAGE_TYPE_2D;
            info.format = windowSurfaceFormat;
            info.layers = 1;
            info.mips = 1;
            info.sampleCount = Samples::SAMPLE_COUNT_1_BIT;
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
            ImageInfo info;
            info.imageType = ImageType::IMAGE_TYPE_2D;
            info.format = bestDepthFormat;
            info.layers = 1;
            info.mips = 1;
            info.sampleCount = Samples::SAMPLE_COUNT_1_BIT;
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
            apiInterface->CreateAttachment(&info, Settings::swapBufferCount, defaultDepthTargetList.data());
            
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

        if (RendererSettings::MSAA_Enabled && RendererSettings::multiSamplingAvailable)
        {
            SetupAttachmentsForMSAA(*RendererSettings::sampleCount);

            uint32_t imagesPerFbo = 4, numFbos = Settings::swapBufferCount;
            uint32_t * imageIds = new uint32_t[numFbos * imagesPerFbo];

            for (uint32_t i = 0, j = 0; i < numFbos * imagesPerFbo; i++, j++)
            {
                imageIds[i++] = msaaRenderTargetList[j]; // Multi sampled color
                imageIds[i++] = defaultRenderTargetList[j]; // default color
                imageIds[i++] = msaaDepthTargetList[j]; // MSAA depth
                imageIds[i] = defaultDepthTargetList[j]; // default depth
            }

            defaultFbos.resize(numFbos);
            apiInterface->CreateFrameBuffer(numFbos, imageIds, imagesPerFbo, defaultRenderPassId,
                Settings::windowWidth, Settings::windowHeight, &defaultFbos[0]);
            
            delete[] imageIds;
        }
        else
        {
            RenderPassAttachmentInfo attchmentDescList[2];

            attchmentDescList[0].finalLayout = ImageLayout::LAYOUT_PRESENT_SRC_KHR;
            attchmentDescList[0].format = Format::B8G8R8A8_UNORM;
            attchmentDescList[0].initialLayout = ImageLayout::LAYOUT_UNDEFINED;
            attchmentDescList[0].loadOp = LoadOperation::LOAD_OP_CLEAR;
            attchmentDescList[0].sampleCount = *RendererSettings::sampleCount;
            attchmentDescList[0].stencilLoadOp = LoadOperation::LOAD_OP_DONT_CARE;
            attchmentDescList[0].stencilLStoreOp = StoreOperation::STORE_OP_DONT_CARE;
            attchmentDescList[0].storeOp = StoreOperation::STORE_OP_STORE;

            attchmentDescList[1].finalLayout = ImageLayout::LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
            attchmentDescList[1].format = bestDepthFormat;
            attchmentDescList[1].initialLayout = ImageLayout::LAYOUT_UNDEFINED;
            attchmentDescList[1].loadOp = LoadOperation::LOAD_OP_CLEAR;
            attchmentDescList[1].sampleCount = *RendererSettings::sampleCount;
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
}

template<typename T>
inline void ForwardRendering<T>::DislogeRenderer()
{
    apiInterface->DestroyFrameBuffer(defaultFbos.data(), (uint32_t)defaultFbos.size());
    apiInterface->DestroyRenderPass(defaultRenderPassId);
    
    {
        {
            bool viewDestroyList[3]{ true, true, true };
            bool freeMemoryList[3]{ false, false, false };
            apiInterface->DestroyAttachment(defaultDepthTargetList.data(), viewDestroyList, freeMemoryList, (uint32_t)defaultDepthTargetList.size());
            apiInterface->FreeAttachmentMemory(&defaultDepthTargetList[0], 1);
        }

        if (RendererSettings::MSAA_Enabled && RendererSettings::multiSamplingAvailable)
        {
            bool viewDestroyList[3]{ true, true, true };
            bool freeMemoryList[3]{ false, false, false};
            apiInterface->DestroyAttachment(msaaRenderTargetList.data(), viewDestroyList, freeMemoryList, (uint32_t)msaaRenderTargetList.size());
            apiInterface->DestroyAttachment(msaaDepthTargetList.data(), viewDestroyList, freeMemoryList, (uint32_t)msaaDepthTargetList.size());

            apiInterface->FreeAttachmentMemory(&msaaDepthTargetList[0], 1);
            apiInterface->FreeAttachmentMemory(&msaaRenderTargetList[0], 1);

        }

        apiInterface->DestroySwapChainImageViews(defaultRenderTargetList.data(), (uint32_t)defaultRenderTargetList.size());
    }
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
    
    /*info.clearColorValue[0] = Settings::clearColorValue[0];
    info.clearColorValue[1] = Settings::clearColorValue[1];
    info.clearColorValue[2] = Settings::clearColorValue[2];
    info.clearColorValue[3] = Settings::clearColorValue[3];
    */
    
    if (RendererSettings::MSAA_Enabled && RendererSettings::multiSamplingAvailable)
    {
        std::array<float, 4> clearColor{ Settings::clearColorValue[0],
            Settings::clearColorValue[1] , Settings::clearColorValue[2] ,
            Settings::clearColorValue[3] };
        
        info.clearColorValue.push_back(clearColor);
        info.clearColorValue.push_back(clearColor);
    }
    else
    {
        std::array<float, 4> clearColor{ Settings::clearColorValue[0],
            Settings::clearColorValue[1] , Settings::clearColorValue[2] ,
            Settings::clearColorValue[3] };

        info.clearColorValue.push_back(clearColor);
    }

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
