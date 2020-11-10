#pragma once

#include "VulkanInterface.h"
#include "DxInterface.h"

#include <vector>

template<typename T>
class ForwardRendering
{
private:
    T * apiInterface;
    std::vector<uint32_t> defaultRenderTargetList, defaultDepthTarget;

public:
    void Init();
    void SetupRenderer();
    void DislogeRenderer();
    void DeInit();
};

#include "RenderingWrapper.h"
#include <Settings.h>

template<typename T>
inline void ForwardRendering<T>::Init()
{
    apiInterface = new T();
    apiInterface->Init();
}

template<typename T>
inline void ForwardRendering<T>::SetupRenderer()
{
    ImageInfo * info = new ImageInfo();
    info->colorSpace = ColorSpace::COLOR_SPACE_SRGB_NONLINEAR_KHR;
    info->degree = Dimensions::Dim2;
    info->format = ImageFormat::B8G8R8A8_UNORM;
    info->height = Settings::windowHeight;
    info->layers = 1;
    info->mips = 1;
    info->sampleCount = Samples::SAMPLE_COUNT_1_BIT;
    info->usage = Usage::USAGE_COLOR_ATTACHMENT_BIT;
    info->width = Settings::windowWidth;

    apiInterface->CreateRenderTarget(info, 3, true, &defaultRenderTargetList);

    info->degree = Dimensions::Dim2;
    info->format = ImageFormat::D32_SFLOAT_S8_UINT;
    info->height = Settings::windowHeight;
    info->layers = 1;
    info->mips = 1;
    info->sampleCount = Samples::SAMPLE_COUNT_1_BIT;
    info->usage = Usage::USAGE_DEPTH_STENCIL_ATTACHMENT_BIT;
    info->width = Settings::windowWidth;

    apiInterface->CreateDepthTarget(info, 3, true, true, &defaultDepthTarget);

    delete info;
}

template<typename T>
inline void ForwardRendering<T>::DislogeRenderer()
{
    apiInterface->DestroyDepthTarget(&defaultDepthTarget, true);
    apiInterface->DestroyRenderTarget(&defaultRenderTargetList, true);
}

template<typename T>
inline void ForwardRendering<T>::DeInit()
{
    apiInterface->DeInit();
    delete apiInterface;
}
