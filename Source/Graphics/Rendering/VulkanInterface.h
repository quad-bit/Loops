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

enum class Format;
enum class CommandBufferLevel;
enum class PipelineType;
enum class CommandPoolProperty;
enum class PipelineStage;
enum class CommandBufferUsage;
enum class MemoryType;
enum class BufferType;
enum class PrimtiveType;
enum class ShaderType;


struct CommandBufferInheritanceInfo;
struct SubmitInfo;
struct PresentInfo;
struct QueueWrapper;
struct BufferInfo;
struct VertexInputAttributeInfo;
struct VertexInputBindingInfo;

struct ShaderStateWrapper;
struct InputAssemblyWrapper;
struct VertexInputWrapper;
struct SetWrapper;

class VkDrawCommandBuffer;

class VulkanInterface
{
private:

    VkAttachmentDescription* UnwrapAttachmentDesc(const RenderPassAttachmentInfo* renderpassAttachmentList, const uint32_t & attachmentCount);
    VkSubpassDescription* UnwrapSubpassDesc(const SubpassInfo* subpassList, const uint32_t & subpassCount);
    VkSubpassDependency* UnwrapSubpassDependency(const SubpassDependency* dependencyList, const uint32_t & dependencyCount);

    VkImageCreateInfo UnwrapImageInfo(ImageInfo * info);
    VkImageViewCreateInfo UnwrapImageViewInfo(ImageInfo * info);
    VkCommandBufferUsageFlagBits UnwrapCommandBufferUsage(const CommandBufferUsage * info);
    VkSubmitInfo * UnwrapSubmitInfo(const SubmitInfo * info);
    VkPipelineStageFlags * UnwrapStageFlags(const PipelineStage * pipelineStage);
    VkMemoryPropertyFlags UnwrapMemoryProperty(const MemoryType * memType);
    VkBufferUsageFlags UnwrapBufferUsageFlags(const BufferType * type);

public:
    VulkanInterface();
    ~VulkanInterface();

    void Init();
    void DeInit();
  
    uint32_t FindBestDepthFormat(Format * imageFormat, const uint32_t & count);

    void CreateRenderTarget(ImageInfo * info, const uint32_t & count, bool defaultTarget,
        std::vector<uint32_t>* ids);
    void DestroyRenderTarget(std::vector<uint32_t>* ids, bool defaultTarget);
    
    void CreateDepthTarget(ImageInfo * info, const uint32_t & count, bool stencilRequired,
        bool defaultTarget, std::vector<uint32_t>* ids);
    void DestroyDepthTarget(std::vector<uint32_t>* ids, bool defaultTarget);

    void CreateRenderPass(
        const RenderPassAttachmentInfo* renderpassAttachmentList, const uint32_t & attachmentCount,
        const SubpassInfo* subpassList, const uint32_t & subpassCount,
        const SubpassDependency* dependencyList, const uint32_t & dependencyCount,
        uint32_t& renderPassId);

    void DestroyRenderPass(const uint32_t & id );

    void CreateFrameBuffer(uint32_t numFrameBuffers, uint32_t * imageViewId, const uint32_t & viewsPerFB,
        uint32_t renderPassId, const uint32_t & width, const uint32_t & height, uint32_t * ids);
    void DestroyFrameBuffer(uint32_t * pid, const uint32_t & count);

    uint32_t CreateCommandPool(PipelineType* pipelineType, CommandPoolProperty * prop);
    void DestroyCommandPool(const uint32_t & poolId);

    uint32_t CreateFence(bool isSignaled);
    uint32_t Create_Semaphore(bool isSignaled);

    void DestroyFence(const uint32_t & id );
    void DestroySemaphore(const uint32_t & id );

    void SetRenderpassBeginInfo(RenderPassBeginInfo * beginInfo, const uint32_t & renderPassId);

    uint32_t GetAvailableSwapchainIndex(const uint32_t & fenceId, const uint32_t & semaphoreId);
    
    void ActivateCommandBuffer(const uint32_t & id);
    
#if (RENDERING_API == VULKAN)
    VkDrawCommandBuffer * CreateCommandBuffer(const uint32_t & poolId, uint32_t * cmdBufferId, CommandBufferLevel* commandBufferLevel, PipelineType bufferType);
#elif (RENDERING_API == DX)
    apiInterface = new DxInterface();
#endif

    void DestroyCommandBuffer(const uint32_t & id );
    void ResetCommandBuffer(const uint32_t & id , const uint32_t & poolId);
    void BeginCommandBufferRecording(const uint32_t & id , const CommandBufferUsage * usage, const CommandBufferInheritanceInfo * inheritanceInfo);
    void EndCommandBufferRecording(const uint32_t & id );

    void SubmitJob(const QueueWrapper * queueWrapper, const SubmitInfo * info, const uint32_t & submitInfoCount, const uint32_t & fenceId);
    void SubmitJob(const SubmitInfo * info, const uint32_t & submitInfoCount);

    void PresentSwapchainImage(const QueueWrapper * queueWrapper, const PresentInfo * info, const uint32_t & presentQueueId);

    bool IsApplicationSafeForClosure();

    uint32_t * CreateBuffer(BufferInfo * info, const uint32_t & count);
    void DestroyBuffer(uint32_t * ids, const uint32_t & count);

    //void InitiateGraphicsPipelineCreation(const uint32_t & meshId, VertexInputAttributeInfo * attribInfo, const uint32_t & attribCount, VertexInputBindingInfo * bindingInfo, const uint32_t & bindingCount);
    //void SetInputAssemblyInfo(const uint32_t & meshId, PrimtiveType * primitive, bool isPrimitiveRestartEnabled);
    //void CreateGraphicsPipeline();
    //void DestroyGraphicsPipeline();

    void GetShaderIds(char ** shaderName, ShaderType * type, uint32_t * id, const uint32_t & shaderCount );
    void CreateVertexInputState(const VertexInputWrapper * vertexInputWrapper);
    void CreateInputAssemblyState(const InputAssemblyWrapper * InputAssemblyWrapper);
    void CreateShaderState(const ShaderStateWrapper * shaderStateWrapper);
    std::vector<SetWrapper *> GetSetsForShaders(const std::vector<std::string> & shaderNames);
    uint32_t CreatePipelineLayout(SetWrapper ** setWrapperList, const size_t & numSets);
};
