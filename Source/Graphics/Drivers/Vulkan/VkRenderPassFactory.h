#pragma once
#include <vector>
#include<vulkan\vulkan.h>
#include <array>

struct RenderPassAttachmentInfo;
struct SubpassInfo;
struct SubpassDependency;

class RenderpassWrapper
{
public:
    uint32_t id;
    VkRenderPass renderPass;
    VkRenderPassBeginInfo beginInfo;
    std::vector<VkClearValue> clearValue;
};

class VkRenderPassFactory
{

private:
    VkRenderPassFactory(){}
    VkRenderPassFactory(VkRenderPassFactory const &) {}
    VkRenderPassFactory const & operator= (VkRenderPassFactory const &) {}

    static VkRenderPassFactory* instance;
    static uint32_t renderpassIdCounter;

    std::vector<VkAttachmentReference> refs;
    uint32_t refCounter;
    std::vector<RenderpassWrapper*> renderpassList;

    uint32_t GetId();

public:
    void Init();
    void DeInit();
    void Update();
    static VkRenderPassFactory* GetInstance();
    ~VkRenderPassFactory();

    void CreateRenderPass(
        const VkAttachmentDescription* renderpassAttachmentList, uint32_t attachmentCount,
        const VkSubpassDescription* subpassList, uint32_t subpassCount,
        const VkSubpassDependency* dependencyList, uint32_t dependencyCount,
        uint32_t& renderPassId
    );

    void CreateRenderPass(
        const VkAttachmentDescription* renderpassAttachmentList, uint32_t attachmentCount,
        const VkSubpassDescription* subpassList, uint32_t subpassCount,
        const VkSubpassDependency* dependencyList, uint32_t dependencyCount,
        const VkRenderPassBeginInfo beginInfo,
        uint32_t& renderPassId
    );
    void DestroyRenderPass(uint32_t id);

    void SetRenderPassBeginInfo(const VkRenderPassBeginInfo beginInfo, uint32_t renderpassId);
    void SetClearColor(std::vector<VkClearValue> clearValue, uint32_t renderPassId);
    
    VkRenderPass * GetRenderPass(uint32_t id);
    std::vector<VkClearValue> * GetClearValue(uint32_t renderpassId);

};
