#pragma once
#include <vulkan\vulkan.h>
#include <vector>
#include <RenderingWrapper.h>

class VulkanGraphicsPipeline;

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

    VkVertexInputAttributeDescription UnwrapVertexInputAttributeInfo(VertexInputAttributeInfo * info);
    VkVertexInputBindingDescription UnwrapVertexInputBindingInfo(VertexInputBindingInfo * info);
    VkPrimitiveTopology UnwrapPrimitiveInfo(PrimtiveType * primitive);

public:
    void Init();
    void DeInit();
    void Update();
    static VulkanGraphicsPipelineFactory* GetInstance();
    ~VulkanGraphicsPipelineFactory();

    std::vector<VkDynamicState> dynamicStateList;

    void InitiatePipelineCreation(uint32_t id, VertexInputAttributeInfo * inputAttributeInfo, const uint32_t &  inputAttribCount,
        VertexInputBindingInfo * inputBindingInfo, const uint32_t & inputBindingCount);

    void SetInputAssemblyInfo(const uint32_t & meshId, PrimtiveType * primitive, bool isPrimitiveRestartEnabled);
    void SetInputAssemblyInfo(PrimtiveType * primitive, bool isPrimitiveRestartEnabled);

};
