#pragma once

#include "Settings.h"
#include "ShaderResourceDescription.h"
#include <vector>
#include <map>
#include <RenderingWrapper.h>

#if (RENDERING_API == VULKAN)
class VulkanInterface;
typedef VulkanInterface ApiInterface;
#elif (RENDERING_API == DX)
class DxInterface;
typedef DxInterface ApiInterface;
#endif

//struct UniformWrapper
//{
//    SetWrapper * setWrapper;
//    BindingWrapper bindingWrapper;
//    uint32_t numUniformsAllocated;
//};

class UniformFactory
{
private:
    UniformFactory(){}
    UniformFactory(UniformFactory const &) {}
    UniformFactory const & operator=(UniformFactory const &) {}

    static UniformFactory* instance;

    std::vector<SetWrapper*> * setWrapperList;
    ApiInterface * apiInterface;

    std::map<uint32_t, GlobalResourceAllocationConfig> setConfig;
    //std::vector<UniformWrapper> uniformWrapperList;
    void HandlerUniformBuffer(ShaderBindingDescription * desc, SetWrapper * setWrapper, BindingWrapper bindingObj, size_t allocationSize);

    std::vector<uint32_t> bufferIds;
    std::vector<uint32_t> memoryIds;

public:
    void Init(ApiInterface * apiInterface);
    void DeInit();
    void Update();
    static UniformFactory* GetInstance();
    ~UniformFactory();
    
    // Create buffers/images
    SetWrapper * AllocateResource(ShaderBindingDescription * desc, size_t * allocationSize, const uint32_t & numBindings, AllocationMethod allocation);
    void UploadDataToBuffers(const uint32_t & bufId, const size_t & dataSize, const size_t & memAlignedSize, void * data, const size_t & memoryOffset, bool keepMemoryMounted);
    void AllocateDescriptors(SetWrapper * wrapper, ShaderBindingDescription * desc, const uint32_t & numBindings, const uint32_t & numDescriptorsPerBinding);

    std::vector<uint32_t> AcquireMeshList(SetWrapper * wrapper);
    size_t GetMemoryAlignedDataSizeForBuffer(const size_t & dataSize);
};
