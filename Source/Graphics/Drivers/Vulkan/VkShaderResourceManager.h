#pragma once

#include <vulkan\vulkan.h>
#include <RenderingWrapper.h>
#include <rapidjson\document.h> 

struct VkDescriptorSetLayoutWrapper
{
    uint32_t id;
    std::vector<uint32_t> setWrapperIds;
    VkDescriptorSetLayoutWrapper * layout;
};

struct ShaderResources
{
    std::string shaderName;
    std::vector<SetWrapper*> setWrappers;
};

class VkShaderResourceManager
{
private:
    VkShaderResourceManager(){}
    VkShaderResourceManager(VkShaderResourceManager const &) {}
    VkShaderResourceManager const & operator= (VkShaderResourceManager const &) {}

    static VkShaderResourceManager* instance;

    uint32_t bindingIdCounter = 0;
    uint32_t setLayoutIdCounter = 0;
    uint32_t GetBindingID();
    uint32_t GetSetLayoutID();
    void DecrementSetLayoutId() { setLayoutIdCounter--; }

    std::vector<SetWrapper*> setWrapperList;
    std::vector<ShaderResources> perShaderResourceList;

    void CreateUniqueSetLayoutWrapper(std::vector<BindingWrapper> & bindingList, std::string shaderName, uint32_t set);
    void AccumulateSetLayoutPerShader(std::string shaderName, SetWrapper * setWrapper);

    VkDescriptorSetLayout * UnwrapSetwrapper(const SetWrapper & setWrapper);
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

    //void CreatePipelineLayout(const std::vector<std::string> & shaderNames )
};
