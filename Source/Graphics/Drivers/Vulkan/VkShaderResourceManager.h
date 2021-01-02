#pragma once

#include <vulkan\vulkan.h>
#include <RenderingWrapper.h>
#include <rapidjson\document.h> 
#include <vector>

struct ShaderResources
{
    std::string shaderName;
    std::vector<SetWrapper*> setWrappers;
};

struct PipelineLayoutWrapper
{
    uint32_t id;
    std::vector<VkDescriptorSetLayout> setLayoutList;
    VkPipelineLayout * pipelineLayout;
};
//
//struct DescriptorSetWrapper
//{
//    uint32_t id;
//    VkDescriptorSet * descriptorSet;
//};
//
//struct VkDescriptorSetLayoutWrapper
//{
//    uint32_t id;
//    VkDescriptorSetLayout * layout;
//};

class VkShaderResourceAllocator;

class VkShaderResourceManager
{
private:
    VkShaderResourceManager(){}
    VkShaderResourceManager(VkShaderResourceManager const &) {}
    VkShaderResourceManager const & operator= (VkShaderResourceManager const &) {}

    static VkShaderResourceManager* instance;
    VkShaderResourceAllocator * resourceAllocator;

    uint32_t bindingIdCounter = 0;
    uint32_t setWrapperIdCounter = 0;
    uint32_t setLayoutWrapperIdCounter = 0;
    uint32_t pipelineLayoutIdCounter = 0;
    uint32_t GetBindingID();
    uint32_t GetSetLayoutID();
    uint32_t GetPipelineLayoutID();
    uint32_t GetVkDescriptorSetLayoutWrapperID();

    void DecrementSetLayoutId() { setWrapperIdCounter--; }

    std::vector<SetWrapper*> setWrapperList;
    //std::vector<VkDescriptorSetLayoutWrapper> setLayoutWrapperList;
    std::map< uint32_t, VkDescriptorSetLayout *> idToSetLayoutMap;
    std::vector<ShaderResources> perShaderResourceList;
    std::vector<PipelineLayoutWrapper> pipelineLayoutWrapperList;

    void CreateUniqueSetLayoutWrapper(std::vector<BindingWrapper> & bindingList, std::string shaderName, uint32_t set);
    void AccumulateSetLayoutPerShader(std::string shaderName, SetWrapper * setWrapper);

    VkDescriptorSetLayout * UnwrapSetwrapper(SetWrapper * setWrapper);
    VkDescriptorType UnwrapDescriptorType(const DescriptorType & descType);

    ShaderType GetTypeFromName(const std::string & shaderName);
    std::string GetShaderNameFromRefl(const std::string & reflName);

public:
    void Init();
    void DeInit();
    void Update();
    static VkShaderResourceManager* GetInstance();
    ~VkShaderResourceManager();

    std::vector<SetWrapper*> GetSetsForShaders(const std::vector<std::string> & shaderNames);
    uint32_t CreatePipelineLayout(SetWrapper ** setWrapperList, const size_t & numSets);

    //void CreatePipelineLayout(const std::vector<std::string> & shaderNames )
};
