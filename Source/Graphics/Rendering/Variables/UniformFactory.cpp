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


void UniformFactory::HandlerUniformBuffer(ShaderBindingDescription * desc, SetWrapper * setWrapper, BindingWrapper bindingObj, size_t allocationSize)
{
    BufferType type { BufferType::UNIFORM_BUFFER_BIT };
    MemoryType mem[2] { MemoryType::HOST_VISIBLE_BIT, MemoryType::HOST_COHERENT_BIT};

    // TODO : remove the hardcoded values and automate it.

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

void UniformFactory::AllocateResource(ShaderBindingDescription * desc, size_t * allocationSize, const uint32_t & numBindings, AllocationMethod allocation)
{
    std::vector<SetWrapper*>::iterator it;
    it = std::find_if(setWrapperList->begin(), setWrapperList->end(), [&](SetWrapper * e)
    {
        return (e->setValue == desc->set && numBindings == e->bindingWrapperList.size());
    });

    ASSERT_MSG(it != setWrapperList->end(), "Set not found");

    for (uint32_t i = 0; i < numBindings; i++)
    {
        BindingWrapper binding = (*it)->bindingWrapperList[i];
        if (binding.bindingName == desc[i].resourceName && binding.bindingObj.binding == desc[i].binding &&
            binding.memberList.size() == desc[i].numElements)
        {
            switch (binding.bindingObj.descriptorType)
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
                HandlerUniformBuffer(&desc[i], *it, binding, allocationSize[i]);
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
        else
        {
            ASSERT_MSG(0, "Binding not found");
        }
    }

    // Allocate descriptor sets
    uint32_t numDescriptorSetsPerSet = Settings::maxFramesInFlight;
    uint32_t * descriptorIds = apiInterface->AllocateDescriptorsForASet((*it), numDescriptorSetsPerSet);

}

void UniformFactory::UploadDataToBuffers(const uint32_t & bufId, const size_t & dataSize, void * data, const size_t & memoryOffset, bool keepMemoryMounted)
{
    apiInterface->CopyBufferDataToMemory(bufId, dataSize, data, memoryOffset);
}
