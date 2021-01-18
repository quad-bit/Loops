#include "UniformFactory.h"
#include <algorithm>
#include <Assertion.h>
#include "RenderingWrapper.h"

#if (RENDERING_API == VULKAN)
#include <VulkanInterface.h>
#elif (RENDERING_API == DX)
#include <DxInterface.h>
#endif

UniformFactory* UniformFactory::instance = nullptr;


void UniformFactory::HandlerUniformBuffer(ShaderResourceDescription * desc, SetWrapper * setWrapper, BindingWrapper bindingObj, size_t allocationSize)
{
    BufferType type { BufferType::UNIFORM_BUFFER_BIT };
    MemoryType mem[2] { MemoryType::HOST_VISIBLE_BIT, MemoryType::HOST_COHERENT_BIT};

    // Create a new buffer
    BufferInfo info = {};
    info.dataSize = allocationSize;
    info.bufType = &type;
    info.memType = mem;
    info.memTypeCount = 2;

    uint32_t * id = apiInterface->CreateBuffers(&info, 1);
    desc->resourceId = *id;
    bufferIds.push_back(*id);
    delete[] id;

    //allocate buffer memory
    uint32_t * memId = apiInterface->AllocateBufferMemory(&desc->resourceId, 1);
    desc->resourceMemoryId = *memId;
    memoryIds.push_back(*memId);
    delete[] memId;
}

void UniformFactory::Init(ApiInterface * api)
{
    this->apiInterface = api;
    setWrapperList = this->apiInterface->GetSetWrapperList();

    //UniformResourceAllocationConfig config = {};
    //config.numResources = 1;
    //config.numDescriptors = Settings::maxFramesInFlight;
    //config.numMemories = 1;
    //config.maxUniqueDescriptorsPerResource = 10; // 10 * maxFramesInFlight descriptors in one uniform buffer

    //setConfig.insert(std::pair<uint32_t, UniformResourceAllocationConfig>({ 0, config }));
    //setConfig.insert(std::pair<uint32_t, UniformResourceAllocationConfig>({ 1, config }));
    //setConfig.insert(std::pair<uint32_t, UniformResourceAllocationConfig>({ 2, config }));
    //setConfig.insert(std::pair<uint32_t, UniformResourceAllocationConfig>({ 3, config }));
    //setConfig.insert(std::pair<uint32_t, UniformResourceAllocationConfig>({ 4, config }));
    //setConfig.insert(std::pair<uint32_t, UniformResourceAllocationConfig>({ 5, config }));
}

void UniformFactory::DeInit()
{
    setConfig.clear();
    {
        apiInterface->DestroyBuffer(bufferIds.data(),(uint32_t) bufferIds.size());
    }

    {
        apiInterface->FreeMemory(memoryIds.data(), (uint32_t)memoryIds.size());
    }
}

void UniformFactory::Update()
{

}

UniformFactory * UniformFactory::GetInstance()
{
    if (instance == nullptr)
    {
        instance = new UniformFactory();
    }
    return instance;
}

UniformFactory::~UniformFactory()
{
}

void UniformFactory::AllocateResource(ShaderResourceDescription * desc, size_t allocationSize, AllocationMethod allocation)
{
    std::vector<SetWrapper*>::iterator it;
    std::vector<BindingWrapper>::iterator itt;

//    BindingWrapper * bindingWrapper;

    it = std::find_if(setWrapperList->begin(), setWrapperList->end(), [&](SetWrapper * e) 
    { 
        if (e->setValue != desc->set)
            return false;
        else 
        {
            itt = std::find_if(e->bindingWrapperList.begin(), e->bindingWrapperList.end(), [&](BindingWrapper b)
            {
                if (b.bindingName == desc->resourceName && b.bindingObj.binding == desc->binding && b.memberList.size() == desc->numElements)
                {
                    return true;
                }
                else
                    return false;
            });
            
            ASSERT_MSG(itt != e->bindingWrapperList.end(), "Binding not found");
            return true;
        }
    });

    ASSERT_MSG(it != setWrapperList->end(), "Set not found");

    switch (itt->bindingObj.descriptorType)
    {
    case DescriptorType::SAMPLER:
        break;
    case DescriptorType::COMBINED_IMAGE_SAMPLER:
        break;
    case DescriptorType::SAMPLED_IMAGE:
        break;
    case DescriptorType::STORAGE_IMAGE:
        break;
    case DescriptorType::UNIFORM_TEXEL_BUFFER:
        break;
    case DescriptorType::STORAGE_TEXEL_BUFFER:
        break;
    case DescriptorType::UNIFORM_BUFFER:
        HandlerUniformBuffer(desc, *it, *itt, allocationSize);
        break;
    case DescriptorType::STORAGE_BUFFER:
        break;
    case DescriptorType::UNIFORM_BUFFER_DYNAMIC:
        break;
    case DescriptorType::STORAGE_BUFFER_DYNAMIC:
        break;
    case DescriptorType::INPUT_ATTACHMENT:
        break;
    }
}
