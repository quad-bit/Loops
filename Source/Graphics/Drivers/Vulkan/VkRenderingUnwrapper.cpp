#include "VkRenderingUnwrapper.h"
#include "VkFrameBufferFactory.h"
#include "VkRenderPassFactory.h"

VkColorSpaceKHR UnWrapColorSpace(ColorSpace colorSpace)
{
    switch (colorSpace)
    {
    case ColorSpace::COLOR_SPACE_SRGB_NONLINEAR_KHR:
        return VK_COLOR_SPACE_SRGB_NONLINEAR_KHR;
    default:
        ASSERT_MSG(0, "colorspace converter not found ");
        std::exit(-1);
    }
}

VkImageViewType  UnWrapImageViewDegree(Dimensions degree)
{
    switch (degree)
    {
    case Dimensions::Dim1:
        return VK_IMAGE_VIEW_TYPE_1D;
        break;
    case Dimensions::Dim2:
        return VK_IMAGE_VIEW_TYPE_2D;
        break;
    default:
        ASSERT_MSG(0, "Converter not found ");
        std::exit(-1);
    }
}

VkImageType  UnWrapImageDegree(Dimensions degree)
{
    switch (degree)
    {
    case Dimensions::Dim1:
        return VK_IMAGE_TYPE_1D;
        break;
    case Dimensions::Dim2:
        return VK_IMAGE_TYPE_2D;
        break;
    default:
        ASSERT_MSG(0, "Converter not found ");
        std::exit(-1);
    }
}

VkFormat UnWrapFormat(Format format)
{
    switch (format)
    {
    case Format::UNDEFINED:
        return VK_FORMAT_UNDEFINED;
        break;

    case Format::B8G8R8A8_UNORM:
        return VK_FORMAT_B8G8R8A8_UNORM;
        break;

    case Format::D16_UNORM:
        return VK_FORMAT_D16_UNORM;
        break;

    case Format::D16_UNORM_S8_UINT:
        return VK_FORMAT_D16_UNORM_S8_UINT;
        break;

    case Format::D24_UNORM_S8_UINT:
        return VK_FORMAT_D24_UNORM_S8_UINT;
        break;

    case Format::D32_SFLOAT:
        return VK_FORMAT_D32_SFLOAT;
        break;

    case Format::D32_SFLOAT_S8_UINT:
        return VK_FORMAT_D32_SFLOAT_S8_UINT;
        break;

    case Format::R32G32B32A32_SFLOAT:
        return VK_FORMAT_R32G32B32A32_SFLOAT;
        break;

    case Format::R32G32B32_SFLOAT:
        return VK_FORMAT_R32G32B32_SFLOAT;
        break;

    default:
        ASSERT_MSG(0, "Converter not found ");
        std::exit(-1);
    }
}

VkImageLayout UnWrapImageLayout(ImageLayout layout)
{
    switch (layout)
    {
    case    ImageLayout::LAYOUT_UNDEFINED:
        return VK_IMAGE_LAYOUT_UNDEFINED;
    case    ImageLayout::LAYOUT_GENERAL:
        return VK_IMAGE_LAYOUT_GENERAL;
    case    ImageLayout::LAYOUT_COLOR_ATTACHMENT_OPTIMAL:
        return VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
    case    ImageLayout::LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL:
        return VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
    case    ImageLayout::LAYOUT_DEPTH_STENCIL_READ_ONLY_OPTIMAL:
        return VK_IMAGE_LAYOUT_DEPTH_STENCIL_READ_ONLY_OPTIMAL;
    case    ImageLayout::LAYOUT_SHADER_READ_ONLY_OPTIMAL:
        return VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
    case    ImageLayout::LAYOUT_TRANSFER_SRC_OPTIMAL:
        return VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;
    case    ImageLayout::LAYOUT_TRANSFER_DST_OPTIMAL:
        return VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
    case    ImageLayout::LAYOUT_PREINITIALIZED:
        return VK_IMAGE_LAYOUT_PREINITIALIZED;
    case    ImageLayout::LAYOUT_DEPTH_READ_ONLY_STENCIL_ATTACHMENT_OPTIMAL:
        return VK_IMAGE_LAYOUT_DEPTH_READ_ONLY_STENCIL_ATTACHMENT_OPTIMAL;
    case    ImageLayout::LAYOUT_DEPTH_ATTACHMENT_STENCIL_READ_ONLY_OPTIMAL:
        return VK_IMAGE_LAYOUT_DEPTH_ATTACHMENT_STENCIL_READ_ONLY_OPTIMAL;
    case    ImageLayout::LAYOUT_DEPTH_ATTACHMENT_OPTIMAL:
        return VK_IMAGE_LAYOUT_DEPTH_ATTACHMENT_OPTIMAL;
    case    ImageLayout::LAYOUT_DEPTH_READ_ONLY_OPTIMAL:
        return VK_IMAGE_LAYOUT_DEPTH_READ_ONLY_OPTIMAL;
    case    ImageLayout::LAYOUT_STENCIL_ATTACHMENT_OPTIMAL:
        return VK_IMAGE_LAYOUT_STENCIL_ATTACHMENT_OPTIMAL;
    case    ImageLayout::LAYOUT_STENCIL_READ_ONLY_OPTIMAL:
        return VK_IMAGE_LAYOUT_STENCIL_READ_ONLY_OPTIMAL;
    case    ImageLayout::LAYOUT_PRESENT_SRC_KHR:
        return VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

    default:
        ASSERT_MSG(0, "Layout converter not found ");
        std::exit(-1);
    }
}

VkAttachmentLoadOp UnWrapLoadOp(LoadOperation op)
{
    switch (op)
    {
    case   LoadOperation::LOAD_OP_LOAD:
        return VK_ATTACHMENT_LOAD_OP_LOAD;
    case   LoadOperation::LOAD_OP_CLEAR:
        return VK_ATTACHMENT_LOAD_OP_CLEAR;
    case   LoadOperation::LOAD_OP_DONT_CARE:
        return VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    default:
        ASSERT_MSG(0, "loadop converter not found ");
        std::exit(-1);
    }
}

VkAttachmentStoreOp UnWrapStoreOp(StoreOperation op)
{
    switch (op)
    {
    case   StoreOperation::STORE_OP_STORE:
        return VK_ATTACHMENT_STORE_OP_STORE;
    case   StoreOperation::STORE_OP_DONT_CARE:
        return VK_ATTACHMENT_STORE_OP_DONT_CARE;
    default:
        ASSERT_MSG(0, "Store op converter not found ");
        std::exit(-1);
    }
}

VkAttachmentReference UnWrapAttachmentRef(AttachmentRef ref)
{
    VkAttachmentReference obj = {};
    obj.attachment = ref.index;
    obj.layout = UnWrapImageLayout(ref.layoutInSubPass);

    return obj;
}

VkSampleCountFlagBits UnWrapSampleCount(Samples sampleCount)
{
    switch (sampleCount)
    {
    case  Samples::SAMPLE_COUNT_1_BIT:
        return VK_SAMPLE_COUNT_1_BIT;

    case  Samples::SAMPLE_COUNT_2_BIT:
        return VK_SAMPLE_COUNT_2_BIT;

    case  Samples::SAMPLE_COUNT_4_BIT:
        return VK_SAMPLE_COUNT_4_BIT;

    case  Samples::SAMPLE_COUNT_8_BIT:
        return VK_SAMPLE_COUNT_8_BIT;

    case  Samples::SAMPLE_COUNT_16_BIT:
        return VK_SAMPLE_COUNT_16_BIT;

    case  Samples::SAMPLE_COUNT_32_BIT:
        return VK_SAMPLE_COUNT_32_BIT;

    case  Samples::SAMPLE_COUNT_64_BIT:
        return VK_SAMPLE_COUNT_64_BIT;

    default:
        ASSERT_MSG(0, "Sample converter not found ");
        std::exit(-1);
    }
}

VkImageUsageFlagBits UnwrapUsage(Usage usage)
{
    switch (usage)
    {
    case Usage::USAGE_TRANSFER_DST_BIT:
        return VkImageUsageFlagBits::VK_IMAGE_USAGE_TRANSFER_DST_BIT;

    case Usage::USAGE_SAMPLED_BIT:
        return VkImageUsageFlagBits::VK_IMAGE_USAGE_SAMPLED_BIT;

    case Usage::USAGE_STORAGE_BIT:
        return VkImageUsageFlagBits::VK_IMAGE_USAGE_STORAGE_BIT;

    case Usage::USAGE_COLOR_ATTACHMENT_BIT:
        return VkImageUsageFlagBits::VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

    case Usage::USAGE_DEPTH_STENCIL_ATTACHMENT_BIT:
        return VkImageUsageFlagBits::VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT;

    case Usage::USAGE_TRANSIENT_ATTACHMENT_BIT:
        return VkImageUsageFlagBits::VK_IMAGE_USAGE_TRANSIENT_ATTACHMENT_BIT;

    case Usage::USAGE_INPUT_ATTACHMENT_BIT:
        return VkImageUsageFlagBits::VK_IMAGE_USAGE_INPUT_ATTACHMENT_BIT;

    case Usage::USAGE_SHADING_RATE_IMAGE_BIT_NV:
        return VkImageUsageFlagBits::VK_IMAGE_USAGE_SHADING_RATE_IMAGE_BIT_NV;

    case Usage::USAGE_FRAGMENT_DENSITY_MAP_BIT_EXT:
        return VkImageUsageFlagBits::VK_IMAGE_USAGE_FRAGMENT_DENSITY_MAP_BIT_EXT;

    default:
        ASSERT_MSG(0, "Usage converter not found ");
        std::exit(-1);
    }
}

VkRenderPassBeginInfo UnwrapRenderPassBeginInfo(RenderPassBeginInfo beginInfo)
{
    VkRenderPassBeginInfo vkBeginInfo = {};
    vkBeginInfo.framebuffer = *VkFrameBufferFactory::GetInstance()->GetFrameBuffer(beginInfo.frameBufferId);
    vkBeginInfo.renderArea.extent.width = (uint32_t)beginInfo.renderAreaExtent[0];
    vkBeginInfo.renderArea.extent.height = (uint32_t)beginInfo.renderAreaExtent[1];
    vkBeginInfo.renderArea.offset.x = (int32_t)beginInfo.renderAreaPosition[0];
    vkBeginInfo.renderArea.offset.y = (int32_t)beginInfo.renderAreaPosition[1];
    vkBeginInfo.renderPass = *VkRenderPassFactory::GetInstance()->GetRenderPass(beginInfo.renderPassId);
    vkBeginInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
    
    if (beginInfo.clearColorValue[0] == -1.0f)
    {
        vkBeginInfo.pClearValues = VkRenderPassFactory::GetInstance()->GetClearValue(beginInfo.renderPassId);
        vkBeginInfo.clearValueCount = VkRenderPassFactory::GetInstance()->GetClearValueCount(beginInfo.renderPassId);
    }
    else
    {
        VkClearValue * clearVal = new VkClearValue[2];
        clearVal[0].color.float32[0] = beginInfo.clearColorValue[0];
        clearVal[0].color.float32[1] = beginInfo.clearColorValue[1];
        clearVal[0].color.float32[2] = beginInfo.clearColorValue[2];
        clearVal[0].color.float32[3] = beginInfo.clearColorValue[3];

        clearVal[1].depthStencil.depth = beginInfo.depthClearValue;
        clearVal[1].depthStencil.stencil = (uint32_t)beginInfo.stencilClearValue;

        vkBeginInfo.pClearValues = clearVal;
        vkBeginInfo.clearValueCount = 2;
    }

    return vkBeginInfo;
}
