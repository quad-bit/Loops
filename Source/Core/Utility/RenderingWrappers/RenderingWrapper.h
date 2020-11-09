#pragma once
#include <stdint.h>

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

struct ImageInfo
{
    uint32_t width, height, mips, layers;
    ImageFormat format;
    Dimensions degree;
    ColorSpace colorSpace;
    Usage usage;
    Samples sampleCount;
};
