#pragma once
#include <vector>
#include<vulkan\vulkan.h>

struct RenderPassAttachmentInfo;
struct SubpassInfo;
struct SubpassDependency;

class RenderpassInfo
{
public:
    uint32_t id;
    VkRenderPass renderPass;
};

class VkRenderPassFactory
{

private:
    VkRenderPassFactory(){}
    VkRenderPassFactory(VkRenderPassFactory const &) {}
    VkRenderPassFactory const & operator= (VkRenderPassFactory const &) {}

    static VkRenderPassFactory* instance;
    static uint32_t renderpassId;

    std::vector<VkAttachmentReference> refs;
    uint32_t refCounter;
    std::vector<RenderpassInfo*> renderpassList;

    VkAttachmentDescription* UnwrapAttachmentDesc(const RenderPassAttachmentInfo* renderpassAttachmentList, uint32_t attachmentCount);
    VkSubpassDescription* UnwrapSubpassDesc(const SubpassInfo* subpassList, uint32_t subpassCount);
    VkSubpassDependency* UnwrapSubpassDependency(const SubpassDependency* dependencyList, uint32_t dependencyCount);

    uint32_t GetId();

public:
    void Init();
    void DeInit();
    void Update();
    static VkRenderPassFactory* GetInstance();
    ~VkRenderPassFactory();

    /*void CreateRenderPass(
        const std::vector<RenderPassAttachmentInfo*> & renderpassAttachmentList,
        const std::vector<SubpassInfo*> & subpassList,
        const std::vector<SubpassDependency*> & dependencyList,
        uint32_t& renderPassId
    );
*/
    
    void CreateRenderPass(
        const RenderPassAttachmentInfo* renderpassAttachmentList, uint32_t attachmentCount,
        const SubpassInfo* subpassList, uint32_t subpassCount,
        const SubpassDependency* dependencyList, uint32_t dependencyCount,
        uint32_t& renderPassId
    );

    void DestroyRenderPass(uint32_t id);

    VkRenderPass * GetRenderPass(uint32_t id);
};
