#pragma once
#include <stdint.h>
#include <Settings.h>
#include <BitArray.h>

enum class Format
{
    UNDEFINED,
    B8G8R8A8_UNORM,
    D32_SFLOAT_S8_UINT, 
    D24_UNORM_S8_UINT, 
    D16_UNORM_S8_UINT, 
    D32_SFLOAT, 
    D16_UNORM,
    R32G32B32A32_SFLOAT,
    R32G32B32_SFLOAT
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

enum class CommandBufferUsage
{
    USAGE_ONE_TIME_SUBMIT_BIT = 0x00000001,
    RENDER_PASS_CONTINUE_BIT = 0x00000002,
    SIMULTANEOUS_USE_BIT = 0x00000004
};

enum class SubpassContentStatus
{
    SUBPASS_CONTENTS_INLINE = 0,
    SUBPASS_CONTENTS_SECONDARY_COMMAND_BUFFERS = 1,
};

enum class PipelineStage
{
    TOP_OF_PIPE_BIT = 0x00000001,
    DRAW_INDIRECT_BIT = 0x00000002,
    VERTEX_INPUT_BIT = 0x00000004,
    VERTEX_SHADER_BIT = 0x00000008,
    TESSELLATION_CONTROL_SHADER_BIT = 0x00000010,
    TESSELLATION_EVALUATION_SHADER_BIT = 0x00000020,
    GEOMETRY_SHADER_BIT = 0x00000040,
    FRAGMENT_SHADER_BIT = 0x00000080,
    EARLY_FRAGMENT_TESTS_BIT = 0x00000100,
    LATE_FRAGMENT_TESTS_BIT = 0x00000200,
    COLOR_ATTACHMENT_OUTPUT_BIT = 0x00000400,
    COMPUTE_SHADER_BIT = 0x00000800,
    TRANSFER_BIT = 0x00001000,
    BOTTOM_OF_PIPE_BIT = 0x00002000,
    HOST_BIT = 0x00004000,
    ALL_GRAPHICS_BIT = 0x00008000,
    ALL_COMMANDS_BIT = 0x00010000,
    TRANSFORM_FEEDBACK_BIT_EXT = 0x01000000,
    CONDITIONAL_RENDERING_BIT_EXT = 0x00040000,
    RAY_TRACING_SHADER_BIT_KHR = 0x00200000,
    ACCELERATION_STRUCTURE_BUILD_BIT_KHR = 0x02000000,
    SHADING_RATE_IMAGE_BIT_NV = 0x00400000,
    TASK_SHADER_BIT_NV = 0x00080000,
    MESH_SHADER_BIT_NV = 0x00100000,
    FRAGMENT_DENSITY_PROCESS_BIT_EXT = 0x00800000,
    COMMAND_PREPROCESS_BIT_NV = 0x00020000,
    FLAG_BITS_MAX_ENUM = 0x7FFFFFFF
};

enum class QueuePurpose
{
    RENDER,
    PRESENT,
    COMPUTE,
    TRANSFER,
    NONE
};

enum class ShaderType
{
    VERTEX,
    TESSELLATION,
    GEOMETRY,
    FRAGMENT,
    COMPUTE
};

enum class BufferType
{
    TRANSFER_SRC_BIT = 0x00000001,
    TRANSFER_DST_BIT = 0x00000002,
    UNIFORM_TEXEL_BUFFER_BIT = 0x00000004,
    STORAGE_TEXEL_BUFFER_BIT = 0x00000008,
    UNIFORM_BUFFER_BIT = 0x00000010,
    STORAGE_BUFFER_BIT = 0x00000020,
    INDEX_BUFFER_BIT = 0x00000040,
    VERTEX_BUFFER_BIT = 0x00000080,
    INDIRECT_BUFFER_BIT = 0x00000100
};

enum class MemoryType
{
    DEVICE_LOCAL_BIT = 0x00000001,
    HOST_VISIBLE_BIT = 0x00000002,
    HOST_COHERENT_BIT = 0x00000004,
    HOST_CACHED_BIT = 0x00000008,
    LAZILY_ALLOCATED_BIT = 0x00000010,
    PROTECTED_BIT = 0x00000020
};

enum class PipelineStates
{
    VertexInputState,
    InputAssemblyState,
    ShaderStage,
    ShaderResourcesLayout,
    TessellationState,
    ViewportState,
    RasterizationState,
    MultisampleState,
    DepthStencilState,
    ColorBlendState,
    DynamicState,
    NumStates
};

enum class VertexIputRate
{
    PerVertex,
    PerInstance
};

enum class PrimtiveType
{
    TOPOLOGY_POINT_LIST = 0,
    TOPOLOGY_LINE_LIST = 1,
    TOPOLOGY_LINE_STRIP = 2,
    TOPOLOGY_TRIANGLE_LIST = 3,
    TOPOLOGY_TRIANGLE_STRIP = 4,
    TOPOLOGY_TRIANGLE_FAN = 5,
    TOPOLOGY_LINE_LIST_WITH_ADJACENCY = 6,
    TOPOLOGY_LINE_STRIP_WITH_ADJACENCY = 7,
    TOPOLOGY_TRIANGLE_LIST_WITH_ADJACENCY = 8,
    TOPOLOGY_TRIANGLE_STRIP_WITH_ADJACENCY = 9,
    TOPOLOGY_PATCH_LIST = 10,
    TOPOLOGY_MAX_ENUM = 0x7FFFFFFF
};

struct MeshInfo
{
    BitArray attribMaskReq;
    bool isIndexed;
    bool bufferPerAttribRequired;
    PrimtiveType * primitive = nullptr;
    bool isPrimitiveRestartEnabled;
};

enum class ShaderResourceClassification
{
    PER_SCENE,
    PER_MATERIAL,
    PER_MATERIAL_INSTANCE
};

#if (RENDERING_API == VULKAN)

    struct ImageInfo
    {
        uint32_t width, height, mips, layers;
        Format format;
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
        Format format;
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
        float clearColorValue[4]{ -1.0f };
        float depthClearValue = -1.0f;
        float stencilClearValue = -1.0f;
        uint32_t frameBufferId;
        uint32_t renderPassId;
        float renderAreaPosition[2];
        float renderAreaExtent[2];
    };

    struct CommandBufferInheritanceInfo
    {

    };

    struct SubmitInfo
    {
        uint32_t waitSemaphoreCount;
        uint32_t * waitSemaphoreIds;
        uint32_t signalSemaphoreCount;
        uint32_t * signalSemaphoreIds;
        uint32_t commandBufferCount;
        uint32_t * commandBufferIds;
        PipelineStage pipelineStage;
        uint32_t queueId;
        PipelineType * queueType;
        QueuePurpose * purpose;
    };

    struct PresentInfo
    {
        uint32_t                 waitSemaphoreCount;
        uint32_t *               pWaitSemaphoreIds;
        uint32_t *               pImageIndices;
    };

    struct QueueWrapper
    {
        uint32_t queueId;
        uint32_t queueFamilyId;
        PipelineType * queueType;
        QueuePurpose * purpose;
    };

    struct BufferInfo
    {
        BufferType * bufType;
        MemoryType * memType;
        void * data;
        size_t dataSize;
        void * pGpuMem;
    };

    struct VertexInputBindingInfo
    {
        uint32_t             binding;
        uint32_t             stride;
        VertexIputRate       inputRate;
    };

    struct VertexInputAttributeInfo 
    {
        uint32_t    location;
        uint32_t    binding;
        Format      format;
        uint32_t    offset;
    };

    struct VertexInputState
    {
        PipelineStates state = PipelineStates::VertexInputState;
        VertexInputBindingInfo * bindingInfo;
        VertexInputAttributeInfo * attribInfo;
        uint32_t bindingCount;
        uint32_t attribCount;
    };

    struct InputAssemblyState
    {
        PipelineStates state = PipelineStates::InputAssemblyState;
        PrimtiveType * primitiveType;
        bool isPrimtiveRestartEnabled;
    };

    enum class DescriptorType
    {
        SAMPLER = 0,
        COMBINED_IMAGE_SAMPLER = 1,
        SAMPLED_IMAGE = 2,
        STORAGE_IMAGE = 3,
        UNIFORM_TEXEL_BUFFER = 4,
        STORAGE_TEXEL_BUFFER = 5,
        UNIFORM_BUFFER = 6,
        STORAGE_BUFFER = 7,
        UNIFORM_BUFFER_DYNAMIC = 8,
        STORAGE_BUFFER_DYNAMIC = 9,
        INPUT_ATTACHMENT = 10,
        NUM_TYPES = 11
    };

    struct DescriptorSetLayoutBinding 
    {
        uint32_t              binding;
        DescriptorType        descriptorType;
        uint32_t              descriptorCount;
        std::vector< ShaderType> stageFlags;
        //const VkSampler*      pImmutableSamplers;
    };

    struct UniformStructMember
    {
        char name[32];
        uint32_t size;
        uint32_t offset;
        char defaultValue[64];
    };

    struct BindingWrapper
    {
        uint32_t id; // not getting used
        DescriptorSetLayoutBinding bindingObj;
        std::vector<UniformStructMember> memberList;
    };

    struct SetWrapper
    {
        uint32_t id;// this is unique
        uint32_t setValue; // this is not unique as another layout can be created for the same set number
        std::vector<BindingWrapper> bindingWrapperList;
        std::vector<std::string> shaderNames;
        std::vector<ShaderType> shaderFlags;
        uint32_t descriptorSetLayoutId;
    };


#elif (RENDERING_API == DX12)

#endif