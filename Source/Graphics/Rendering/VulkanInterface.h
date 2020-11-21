#pragma once
#include <stdint.h>
#include <vector>
#include <vulkan\vulkan.h>

using namespace std;

struct ImageInfo;
struct RenderPassAttachmentInfo;
struct SubpassInfo;
struct SubpassDependency;
struct RenderPassBeginInfo;

enum class ImageFormat;
enum class CommandBufferLevel;
enum class PipelineType;
enum class CommandPoolProperty;

class VkDrawCommandBuffer;

class VulkanInterface
{
private:

    VkAttachmentDescription* UnwrapAttachmentDesc(const RenderPassAttachmentInfo* renderpassAttachmentList, uint32_t attachmentCount);
    VkSubpassDescription* UnwrapSubpassDesc(const SubpassInfo* subpassList, uint32_t subpassCount);
    VkSubpassDependency* UnwrapSubpassDependency(const SubpassDependency* dependencyList, uint32_t dependencyCount);

    VkImageCreateInfo UnwrapImageInfo(ImageInfo * info);
    VkImageViewCreateInfo UnwrapImageViewInfo(ImageInfo * info);

public:
    VulkanInterface();
    ~VulkanInterface();

    void Init();
    void DeInit();
  
    uint32_t FindBestDepthFormat(ImageFormat * imageFormat, uint32_t count);

    void CreateRenderTarget(ImageInfo * info, uint32_t count, bool defaultTarget, 
        std::vector<uint32_t>* ids);
    void DestroyRenderTarget(std::vector<uint32_t>* ids, bool defaultTarget);
    
    void CreateDepthTarget(ImageInfo * info, uint32_t count, bool stencilRequired, 
        bool defaultTarget, std::vector<uint32_t>* ids);
    void DestroyDepthTarget(std::vector<uint32_t>* ids, bool defaultTarget);

    void CreateRenderPass(
        const RenderPassAttachmentInfo* renderpassAttachmentList, uint32_t attachmentCount,
        const SubpassInfo* subpassList, uint32_t subpassCount,
        const SubpassDependency* dependencyList, uint32_t dependencyCount,
        uint32_t& renderPassId);

    void DestroyRenderPass(uint32_t id);

    void CreateFrameBuffer(uint32_t numFrameBuffers, uint32_t * imageViewId, uint32_t viewsPerFB,
        uint32_t renderPassId, uint32_t width, uint32_t height, uint32_t * ids);
    void DestroyFrameBuffer(uint32_t * pid, uint32_t count);

    uint32_t CreateCommandPool(PipelineType* pipelineType, CommandPoolProperty * prop);
    void DestroyCommandPool(uint32_t poolId);

    uint32_t CreateFence(bool isSignaled);
    uint32_t Create_Semaphore(bool isSignaled);

    void DestroyFence(uint32_t id);
    void DestroySemaphore(uint32_t id);

    void SetRenderpassBeginInfo(RenderPassBeginInfo * beginInfo, uint32_t renderPassId);

    uint32_t GetAvailableSwapchainIndex(uint32_t fenceId, uint32_t semaphoreId);
    
    void ActivateCommandBuffer(uint32_t index);
    
#if (RENDERING_API == VULKAN)
    VkDrawCommandBuffer * CreateCommandBuffer(const uint32_t & poolId, uint32_t * cmdBufferId, CommandBufferLevel* commandBufferLevel, PipelineType bufferType);
#elif (RENDERING_API == DX)
    apiInterface = new DxInterface();
#endif

    void DestroyCommandBuffer(uint32_t id);
    void ResetCommandBuffer(uint32_t id, uint32_t poolId);
};
