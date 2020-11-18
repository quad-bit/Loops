#include "VkRenderPassFactory.h"
#include "VkRenderingUnwrapper.h"
#include "VulkanUtility.h"
#include <Assertion.h>
#include <algorithm>

VkRenderPassFactory* VkRenderPassFactory::instance = nullptr;
uint32_t VkRenderPassFactory::renderpassId = 0;
/*
VkAttachmentDescription* VkRenderPassFactory::UnwrapAttachmentDesc(const RenderPassAttachmentInfo* renderpassAttachmentList, uint32_t attachmentCount)
{
    VkAttachmentDescription* attachmentDescriptions = new VkAttachmentDescription[attachmentCount];

    for(uint32_t i = 0; i < attachmentCount; i++)
    {
        const RenderPassAttachmentInfo* obj = renderpassAttachmentList + i;
        attachmentDescriptions[i].initialLayout = UnWrapImageLayout(obj->initialLayout);
        attachmentDescriptions[i].finalLayout = UnWrapImageLayout(obj->finalLayout);
        attachmentDescriptions[i].format = UnWrapFormat(obj->format);
        attachmentDescriptions[i].loadOp = UnWrapLoadOp(obj->loadOp);
        attachmentDescriptions[i].storeOp = UnWrapStoreOp(obj->storeOp);
        attachmentDescriptions[i].stencilLoadOp = UnWrapLoadOp(obj->stencilLoadOp);
        attachmentDescriptions[i].stencilStoreOp = UnWrapStoreOp(obj->stencilLStoreOp);
        attachmentDescriptions[i].samples = UnWrapSampleCount(obj->sampleCount);
        attachmentDescriptions[i].flags = 0;
    }

    return attachmentDescriptions;
}

VkSubpassDescription* VkRenderPassFactory::UnwrapSubpassDesc(const SubpassInfo* subpassList, uint32_t subpassCount)
{
    VkSubpassDescription * subpassDescriptions = new VkSubpassDescription[subpassCount];

    //for each (SubpassInfo* obj in subpassList)
    for (uint32_t i = 0; i < subpassCount; i++)
    {
        const SubpassInfo* obj = subpassList + i;

        //Color
        if (obj->colorAttachmentCount > 0)
        {
            subpassDescriptions[i].colorAttachmentCount = obj->colorAttachmentCount;
            VkAttachmentReference* refCol = new VkAttachmentReference[obj->colorAttachmentCount];
            for (uint32_t j = 0; j < subpassDescriptions[i].colorAttachmentCount; j++)
            {
                AttachmentRef * aref = obj->pColorAttachments + j;
                refCol[i] = UnWrapAttachmentRef(*aref);
                //refs.push_back(ref);
            }
            subpassDescriptions[i].pColorAttachments = refCol;
            //refCounter += obj->colorAttachmentCount;
        }
        else
        {
            subpassDescriptions[i].pColorAttachments = nullptr;
            subpassDescriptions[i].colorAttachmentCount = 0;
        }

        //Input
        if (obj->inputAttachmentCount > 0)
        {
            subpassDescriptions[i].inputAttachmentCount = obj->inputAttachmentCount;
            VkAttachmentReference* refInp = new VkAttachmentReference[obj->inputAttachmentCount];

            for (uint32_t j = 0; j < subpassDescriptions[i].inputAttachmentCount; j++)
            {
                AttachmentRef * aref = obj->pInputAttachments + j;
                refInp[i] = UnWrapAttachmentRef(*aref);
            }
            subpassDescriptions[i].pInputAttachments = refInp;
            //refCounter += obj->inputAttachmentCount;
        }
        else
        {
            subpassDescriptions[i].pInputAttachments = nullptr;
            subpassDescriptions[i].inputAttachmentCount = 0;
        }
        
        //Depth
        if (obj->pDepthStencilAttachment != nullptr)
        {
            VkAttachmentReference *refDepth = new VkAttachmentReference;
            *refDepth = UnWrapAttachmentRef(*obj->pDepthStencilAttachment);

            subpassDescriptions[i].pDepthStencilAttachment = refDepth;
        }
        else
        {
            subpassDescriptions[i].pDepthStencilAttachment = nullptr;
        }
        //resolve, the count is missing.. might require another vector
        subpassDescriptions[i].pResolveAttachments = nullptr;
        subpassDescriptions[i].preserveAttachmentCount = 0;
        subpassDescriptions[i].pPreserveAttachments = nullptr;
        
        subpassDescriptions[i].pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
        subpassDescriptions[i].flags = 0;
    }

    return subpassDescriptions;
}

VkSubpassDependency* VkRenderPassFactory::UnwrapSubpassDependency(const SubpassDependency* dependencyList, uint32_t dependencyCount)
{
    return nullptr;
}
*/
uint32_t VkRenderPassFactory::GetId()
{
    return renderpassId++;
}

void VkRenderPassFactory::Init()
{
    refCounter = 0;
}

void VkRenderPassFactory::DeInit()
{
    for (uint32_t i = 0; i < renderpassList.size(); i++)
    {
        delete renderpassList[i];
    }
    renderpassList.clear();
}

void VkRenderPassFactory::Update()
{

}

VkRenderPassFactory * VkRenderPassFactory::GetInstance()
{
    if (instance == nullptr)
    {
        instance = new VkRenderPassFactory();
    }
    return instance;
}

VkRenderPassFactory::~VkRenderPassFactory()
{

}
/*
void VkRenderPassFactory::CreateRenderPass(
    const RenderPassAttachmentInfo* renderpassAttachmentList, uint32_t attachmentCount,
    const SubpassInfo* subpassList, uint32_t subpassCount,
    const SubpassDependency* dependencyList, uint32_t dependencyCount,
    uint32_t& renderPassId)

{
    VkAttachmentDescription* attachmentDescList = UnwrapAttachmentDesc(renderpassAttachmentList, attachmentCount);
    VkSubpassDescription* subpassDescList = UnwrapSubpassDesc(subpassList, subpassCount);

    RenderpassInfo * info = new RenderpassInfo();
    info->id = GetId();
    
    VkRenderPassCreateInfo renderPassCreateInfo{};
    renderPassCreateInfo.attachmentCount = attachmentCount;
    renderPassCreateInfo.dependencyCount = dependencyCount;
    renderPassCreateInfo.pAttachments = attachmentDescList;
    renderPassCreateInfo.pDependencies = nullptr;
    renderPassCreateInfo.pSubpasses = subpassDescList;
    renderPassCreateInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
    renderPassCreateInfo.subpassCount = subpassCount;
    
    ErrorCheck(vkCreateRenderPass(*CoreObjects::logicalDeviceObj, &renderPassCreateInfo, CoreObjects::pAllocator, &info->renderPass));

    renderpassList.push_back(info);
    renderpassId = info->id;

    if (attachmentDescList != nullptr)
        delete [] attachmentDescList;

    if (subpassDescList->pColorAttachments != nullptr)
        delete[] subpassDescList->pColorAttachments;

    if (subpassDescList->pDepthStencilAttachment != nullptr)
        delete subpassDescList->pDepthStencilAttachment;
    
    if (subpassDescList->pInputAttachments != nullptr)
        delete[] subpassDescList->pInputAttachments;

    if (subpassDescList->pResolveAttachments != nullptr)
        delete[] subpassDescList->pResolveAttachments;

    if (subpassDescList->pPreserveAttachments != nullptr)
        delete[] subpassDescList->pPreserveAttachments;

    if (subpassDescList != nullptr)
        delete[] subpassDescList;

}
*/

void VkRenderPassFactory::CreateRenderPass(const VkAttachmentDescription * renderpassAttachmentList, uint32_t attachmentCount, const VkSubpassDescription * subpassList, uint32_t subpassCount, const VkSubpassDependency * dependencyList, uint32_t dependencyCount, uint32_t & renderPassId)
{
    RenderpassInfo * info = new RenderpassInfo();
    info->id = GetId();

    VkRenderPassCreateInfo renderPassCreateInfo{};
    renderPassCreateInfo.attachmentCount = attachmentCount;
    renderPassCreateInfo.dependencyCount = dependencyCount;
    renderPassCreateInfo.pAttachments = renderpassAttachmentList;
    renderPassCreateInfo.pDependencies = nullptr;
    renderPassCreateInfo.pSubpasses = subpassList;
    renderPassCreateInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
    renderPassCreateInfo.subpassCount = subpassCount;

    ErrorCheck(vkCreateRenderPass(*CoreObjects::logicalDeviceObj, &renderPassCreateInfo, CoreObjects::pAllocator, &info->renderPass));

    renderpassList.push_back(info);
    renderpassId = info->id;
}

void VkRenderPassFactory::DestroyRenderPass(uint32_t id)
{
    std::vector<RenderpassInfo*>::iterator it;
    it = std::find_if(renderpassList.begin(), renderpassList.end(), [&](RenderpassInfo * e) { return e->id == id; });

    ASSERT_MSG(it != renderpassList.end(), "Image id not found");

    vkDestroyRenderPass(*CoreObjects::logicalDeviceObj, (*it)->renderPass, CoreObjects::pAllocator);
}

VkRenderPass * VkRenderPassFactory::GetRenderPass(uint32_t id)
{
    std::vector<RenderpassInfo*>::iterator it;
    it = std::find_if(renderpassList.begin(), renderpassList.end(), [&](RenderpassInfo * e) { return e->id == id; });

    ASSERT_MSG(it != renderpassList.end(), "Image id not found");

    return &(*it)->renderPass;
}
