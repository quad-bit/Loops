#ifndef RenderingUnwrapper_H
#define RenderingUnwrapper_H

#include <RenderingWrapper.h>
#include <Assertion.h>
#include <vulkan\vulkan.h>

VkColorSpaceKHR UnWrapColorSpace(ColorSpace colorSpace);

VkImageViewType  UnWrapImageViewDegree(Dimensions degree);

VkImageType  UnWrapImageDegree(Dimensions degree);

VkFormat UnWrapFormat(ImageFormat format);

VkImageLayout UnWrapImageLayout(ImageLayout layout);

VkAttachmentLoadOp UnWrapLoadOp(LoadOperation op);

VkAttachmentStoreOp UnWrapStoreOp(StoreOperation op);

VkAttachmentReference UnWrapAttachmentRef(AttachmentRef ref);

VkSampleCountFlagBits UnWrapSampleCount(Samples sampleCount);

VkImageUsageFlagBits UnwrapUsage(Usage usage);

#endif RenderingUnwrapper_H