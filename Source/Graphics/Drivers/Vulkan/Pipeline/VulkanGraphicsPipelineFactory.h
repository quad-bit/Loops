#pragma once
#include <vulkan\vulkan.h>
#include <vector>
#include <map>
#include <RenderingWrapper.h>
#include <PipelineStateWrappers.h>

class VulkanGraphicsPipeline;
class Shader;

struct VkShaderStageWrapper
{
    uint32_t id;
    VkPipelineShaderStageCreateInfo * shaderStageCreateInfo;
    uint32_t shaderCount;
};

class VulkanGraphicsPipelineFactory
{

private:
    VulkanGraphicsPipelineFactory(){}
    VulkanGraphicsPipelineFactory(VulkanGraphicsPipelineFactory const &) {}
    VulkanGraphicsPipelineFactory const & operator= (VulkanGraphicsPipelineFactory const &) {}

    static VulkanGraphicsPipelineFactory* instance;

    VkPipelineCache                                     pipelineCacheObj;
    VkPipelineShaderStageCreateInfo                     pipelineShaderStageCreateInfo = {};
    VkPipelineVertexInputStateCreateInfo                pipelineVertexInputStateCreateInfo = {};
    VkPipelineInputAssemblyStateCreateInfo              pipelineInputAssemblyStateCreateInfo = {};
    VkPipelineTessellationStateCreateInfo               pipelineTessellationStateCreateInfo = {};
    VkPipelineViewportStateCreateInfo                   pipelineViewportStateCreateInfoObj = {};
    VkPipelineRasterizationStateCreateInfo              pipelineRasterizationStateCreateInfoObj = {};
    VkPipelineMultisampleStateCreateInfo                pipelineMultisampleStateCreateInfo = {};
    VkPipelineDepthStencilStateCreateInfo               pipelineDepthStencilStateCreateInfo = {};
    VkPipelineColorBlendStateCreateInfo                 pipelineColorBlendStateCreateInfoObj = {};
    VkPipelineColorBlendAttachmentState                 attachments[1];
    VkPipelineDynamicStateCreateInfo                    pipelineDynamicStateCreateInfoObj = {};
    VkGraphicsPipelineCreateInfo                        vulkanGraphicsPipelineCreateInfoObj = {};

    void InitPipelineCache();
    void CreatePipelineShaderStage();
    void CreatePipelineVertexInputState();
    void CreatePipelineInputAssemblyState();
    void CreatePipelineTessellationState();
    void CreatePipelineViewportState();
    void CreatePipelineRasterizationState();
    void CreatePipelineMultiSampleState();
    void CreatePipelineDepthStencilState();
    void CreatePipelineColorBlendState();
    void CreateDynamicState();

    std::vector<VulkanGraphicsPipeline *> pipelineList;
    std::map<uint32_t, VkPipelineVertexInputStateCreateInfo *> idToVertexInputMap;
    std::map<uint32_t, VkPipelineInputAssemblyStateCreateInfo *> idToInputAssemblyMap;
    std::vector<VkShaderStageWrapper *> shaderStageWrapperList;

    //deprecated
    VkVertexInputAttributeDescription UnwrapVertexInputAttributeInfo(VertexInputAttributeInfo * info);
    //deprecated
    VkVertexInputBindingDescription UnwrapVertexInputBindingInfo(VertexInputBindingInfo * info);
    
    VkVertexInputAttributeDescription * UnwrapVertexInputAttributeInfo(VertexInputAttributeInfo * info, const uint32_t & count);
    VkVertexInputBindingDescription * UnwrapVertexInputBindingInfo(VertexInputBindingInfo * info, const uint32_t & count);
    VkPrimitiveTopology UnwrapPrimitiveInfo(PrimtiveType * primitive);

public:
    void Init();
    void DeInit();
    void Update();
    static VulkanGraphicsPipelineFactory* GetInstance();
    ~VulkanGraphicsPipelineFactory();

    std::vector<VkDynamicState> dynamicStateList;

    void CreateVertexInputState(const VertexInputWrapper * vertexInputWrapper);
    void CreateInputAssemblyState(const InputAssemblyWrapper * InputAssemblyWrapper);
    void CreateShaderState(const ShaderStateWrapper * shaderStateWrapper);
};
