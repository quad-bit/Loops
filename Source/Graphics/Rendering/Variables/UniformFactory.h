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

struct BufferWrapper
{
    uint32_t bufferId;
    uint32_t memoryId;
    size_t bufferSize;
    size_t memorySize;
    size_t memoryDivisionUnit;
};

struct UniformWrapper
{
    SetWrapper * setWrapper;
    BindingWrapper bindingWrapper;
    uint32_t numUniformsAllocated;
};

class UniformFactory
{
private:
    UniformFactory(){}
    UniformFactory(UniformFactory const &) {}
    UniformFactory const & operator=(UniformFactory const &) {}

    static UniformFactory* instance;

    std::vector<SetWrapper*> * setWrapperList;
    ApiInterface * apiInterface;

    std::map<uint32_t, UniformResourceAllocationConfig> setConfig;
    std::vector<UniformWrapper> uniformWrapperList;
    void HandlerUniformBuffer(ShaderResourceDescription * desc, SetWrapper * setWrapper, BindingWrapper bindingObj, size_t allocationSize);

    std::vector<uint32_t> bufferIds;
    std::vector<uint32_t> memoryIds;

public:
    void Init(ApiInterface * apiInterface);
    void DeInit();
    void Update();
    static UniformFactory* GetInstance();
    ~UniformFactory();

    void AllocateResource(ShaderResourceDescription * desc, size_t allocationSize, AllocationMethod allocation);
};
