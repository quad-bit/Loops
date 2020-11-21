#pragma once
#include <stdint.h>
#include <Settings.h>

enum class ImageFormat
{
    UNDEFINED,
    B8G8R8A8_UNORM,
    D32_SFLOAT_S8_UINT, 
    D24_UNORM_S8_UINT, 
    D16_UNORM_S8_UINT, 
    D32_SFLOAT, 
    D16_UNORM
};

enum class ColorSpace
{
    COLOR_SPACE_SRGB_NONLINEAR_KHR
};

enum class Dimensions
{
    Dim1,
    Dim2,
    Dim3
};

enum class Usage
{
    USAGE_TRANSFER_SRC_BIT = 0x00000001,
    USAGE_TRANSFER_DST_BIT = 0x00000002,
    USAGE_SAMPLED_BIT = 0x00000004,
    USAGE_STORAGE_BIT = 0x00000008,
    USAGE_COLOR_ATTACHMENT_BIT = 0x00000010,
    USAGE_DEPTH_STENCIL_ATTACHMENT_BIT = 0x00000020,
    USAGE_TRANSIENT_ATTACHMENT_BIT = 0x00000040,
    USAGE_INPUT_ATTACHMENT_BIT = 0x00000080,
    USAGE_SHADING_RATE_IMAGE_BIT_NV = 0x00000100,
    USAGE_FRAGMENT_DENSITY_MAP_BIT_EXT = 0x00000200,
};

enum class Samples
{
    SAMPLE_COUNT_1_BIT = 0x00000001,
    SAMPLE_COUNT_2_BIT = 0x00000002,
    SAMPLE_COUNT_4_BIT = 0x00000004,
    SAMPLE_COUNT_8_BIT = 0x00000008,
    SAMPLE_COUNT_16_BIT = 0x00000010,
    SAMPLE_COUNT_32_BIT = 0x00000020,
    SAMPLE_COUNT_64_BIT = 0x00000040,
};

enum class ImageLayout
{
    LAYOUT_UNDEFINED,
    LAYOUT_GENERAL,
    LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
    LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL,
    LAYOUT_DEPTH_STENCIL_READ_ONLY_OPTIMAL,
    LAYOUT_SHADER_READ_ONLY_OPTIMAL,
    LAYOUT_TRANSFER_SRC_OPTIMAL,
    LAYOUT_TRANSFER_DST_OPTIMAL,
    LAYOUT_PREINITIALIZED ,
    LAYOUT_DEPTH_READ_ONLY_STENCIL_ATTACHMENT_OPTIMAL ,
    LAYOUT_DEPTH_ATTACHMENT_STENCIL_READ_ONLY_OPTIMAL ,
    LAYOUT_DEPTH_ATTACHMENT_OPTIMAL ,
    LAYOUT_DEPTH_READ_ONLY_OPTIMAL,
    LAYOUT_STENCIL_ATTACHMENT_OPTIMAL,
    LAYOUT_STENCIL_READ_ONLY_OPTIMAL,
    LAYOUT_PRESENT_SRC_KHR
};

enum class LoadOperation
{
    LOAD_OP_LOAD = 0,
    LOAD_OP_CLEAR = 1,
    LOAD_OP_DONT_CARE = 2,
};

enum class StoreOperation
{
    STORE_OP_STORE = 0,
    STORE_OP_DONT_CARE = 1,
};

enum class CommandPoolProperty
{
    TRANSIENT_BIT = 0x00000001,
    RESET_COMMAND_BUFFER_BIT = 0x00000002,
    TRANS_RESET = 0x00000003
};

enum class CommandBufferLevel
{
    PRIMARY = 0,
    SECONDARY = 1
};

enum class CommandBufferProperty
{
    SHORT_LIVED = 0,
    RESET_ALLOWED = 1
};

enum class PipelineType
{
    GRAPHICS,
    COMPUTE,
    TRANSFER,
};


#if (RENDERING_API == VULKAN)

    struct ImageInfo
    {
        uint32_t width, height, mips, layers;
        ImageFormat format;
        Dimensions degree;
        ColorSpace colorSpace;
        Usage usage;
        Samples sampleCount;
        ImageLayout initialLayout;
    };

    struct RenderPassAttachmentInfo
    {
        RenderPassAttachmentInfo()
        {
            static uint32_t counter = 0;
            id = counter++;
        }
        uint32_t id;
        ImageFormat format;
        Samples sampleCount;
        LoadOperation loadOp;
        StoreOperation storeOp;
        LoadOperation stencilLoadOp;
        StoreOperation stencilLStoreOp;
        ImageLayout initialLayout, finalLayout;
    };

    struct AttachmentRef
    {
        uint32_t index;
        ImageLayout layoutInSubPass;
    };

    struct SubpassInfo
    {
        SubpassInfo()
        {
            static uint32_t counter = 0;
            id = counter++;

            inputAttachmentCount = 0;
            pInputAttachments = nullptr;
            colorAttachmentCount = 0;
            pColorAttachments = nullptr;
            pResolveAttachments = nullptr;
            pDepthStencilAttachment = nullptr;
        }
        uint32_t                        id;
        uint32_t                        inputAttachmentCount;
        AttachmentRef *                 pInputAttachments;
        uint32_t                        colorAttachmentCount;
        AttachmentRef *                 pColorAttachments;
        AttachmentRef *                 pResolveAttachments;
        AttachmentRef *                 pDepthStencilAttachment;
    };

    struct SubpassDependency
    {
        SubpassDependency()
        {
            static uint32_t counter = 0;
            id = counter++;
        }
        uint32_t id;
    };

    struct RenderPassBeginInfo
    {
        float clearColorValue[4];
        float depthClearValue;
        float stencilClearValue;
        uint32_t frameBufferId;
        uint32_t renderPassId;
        float renderAreaPosition[2];
        float renderAreaExtent[2];
    };


#elif (RENDERING_API == DX12)

#endif