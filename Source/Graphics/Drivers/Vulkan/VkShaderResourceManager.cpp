#include "VkShaderResourceManager.h"
#include "FileReader.h"
#include <stdio.h>
#include "VulkanUtility.h"
#include "VulkanManager.h"
#include <Assertion.h>
#include <HashManager.h>
#include <VkRenderingUnwrapper.h>

using namespace rapidjson;

VkShaderResourceManager* VkShaderResourceManager::instance = nullptr;

uint32_t GetGPUAlignedSize(uint32_t unalignedSize)
{
    size_t uboAlignment = VulkanManager::GetInstance()->GetPhysicalDeviceProps().limits.minUniformBufferOffsetAlignment;//vkh::GContext.gpu.deviceProps.limits.minUniformBufferOffsetAlignment;
    return static_cast<uint32_t>((unalignedSize / uboAlignment) * uboAlignment + ((unalignedSize % uboAlignment) > 0 ? uboAlignment : 0));
}

DescriptorType StringToDescType(const char* str)
{
    if (!strcmp(str, "UNIFORM")) return DescriptorType::UNIFORM_BUFFER;
    if (!strcmp(str, "SAMPLER")) return DescriptorType::SAMPLER;
    if (!strcmp(str, "COMBINED_IMAGE_SAMPLER")) return DescriptorType::COMBINED_IMAGE_SAMPLER;
    if (!strcmp(str, "SAMPLED_IMAGE")) return DescriptorType::SAMPLED_IMAGE;

    ASSERT_MSG(0 , "trying to convert an invalid string to input type");
    return DescriptorType::NUM_TYPES;
}

uint32_t VkShaderResourceManager::GetBindingID()
{
    return bindingIdCounter++;
}

uint32_t VkShaderResourceManager::GetSetLayoutID()
{
    return setLayoutIdCounter++;
}

void VkShaderResourceManager::CreateUniqueSetLayoutWrapper(std::vector<BindingWrapper> & bindingList, std::string shaderName, uint32_t set)
{
    SetWrapper * setWrapper = new SetWrapper;
    setWrapper->bindingWrapperList = bindingList;
    setWrapper->setValue = set;
    setWrapper->id = GetSetLayoutID();

    // use the hash of the set wrapper to check its existence
    int result = HashManager::GetInstance()->FindDescriptorSetHash(setWrapper, setWrapper->id);
    // if exists add the shader name to that object's list
    if (result != -1)
    {
        std::vector<SetWrapper*>::iterator it;
        it = std::find_if(setWrapperList.begin(), setWrapperList.end(), [&](SetWrapper* e) { return e->id == result; });

        if (it == setWrapperList.end())
        {
            ASSERT_MSG(0, "Id mismatch");
        }

        (*it)->shaderNames.push_back(shaderName);
        (*it)->shaderFlags.push_back(GetTypeFromName(shaderName));

        DecrementSetLayoutId();

        AccumulateSetLayoutPerShader(shaderName, (*it));

        delete setWrapper;
    }
    else
    {
        // if not add this new object to the wrapper list
        setWrapper->shaderNames.push_back(shaderName);
        setWrapper->shaderFlags.push_back(GetTypeFromName(shaderName));
        setWrapperList.push_back(setWrapper);
        
        AccumulateSetLayoutPerShader(shaderName, setWrapper);
    }
}

void VkShaderResourceManager::AccumulateSetLayoutPerShader(std::string reflName, SetWrapper * setWrapper)
{
    std::string glslShader = GetShaderNameFromRefl(reflName);

    std::vector<ShaderResources>::iterator it;
    it = std::find_if(perShaderResourceList.begin(), perShaderResourceList.end(), [&](ShaderResources e) { return e.shaderName == glslShader; });

    if (it == perShaderResourceList.end())
    {
        ShaderResources obj = {};
        obj.shaderName = glslShader;
        obj.setWrappers.push_back(setWrapper);
        perShaderResourceList.push_back(obj);
    }
    else
    {
        it->setWrappers.push_back(setWrapper);
    }
}

//deprecated
VkDescriptorSetLayout * VkShaderResourceManager::UnwrapSetwrapper(const SetWrapper & setWrapper)
{
    std::vector<VkDescriptorSetLayoutBinding> bindingList;
    uint32_t numBindings = (uint32_t)setWrapper.bindingWrapperList.size();
    bindingList.resize(numBindings);

    for (uint32_t i = 0; i < numBindings; i++)
    {
       DescriptorSetLayoutBinding obj = setWrapper.bindingWrapperList[i].bindingObj;
       bindingList[i].binding = obj.binding;
       bindingList[i].descriptorCount = obj.descriptorCount;
       bindingList[i].descriptorType = UnwrapDescriptorType(obj.descriptorType);
       bindingList[i].stageFlags = UnwrapShaderStage(obj.stageFlags.data(), (uint32_t)obj.stageFlags.size());
       bindingList[i].pImmutableSamplers = nullptr;
    }

    VkDescriptorSetLayout * layout = new VkDescriptorSetLayout;
    VkDescriptorSetLayoutCreateInfo info = {};
    info.bindingCount = numBindings;
    info.pBindings = bindingList.data();
    info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
    info.pNext = nullptr;

    ErrorCheck(vkCreateDescriptorSetLayout(*CoreObjects::logicalDeviceObj, &info, CoreObjects::pAllocator, layout));

    return layout;
}

VkDescriptorType VkShaderResourceManager::UnwrapDescriptorType(const DescriptorType & descType)
{
    VkDescriptorType vkType;

    switch (descType)
    {
        case DescriptorType::SAMPLER:
            vkType = VkDescriptorType::VK_DESCRIPTOR_TYPE_SAMPLER;
            break;
        case DescriptorType::COMBINED_IMAGE_SAMPLER:
            vkType = VkDescriptorType::VK_DESCRIPTOR_TYPE_SAMPLER;
            break;
        case DescriptorType::SAMPLED_IMAGE:
            vkType = VkDescriptorType::VK_DESCRIPTOR_TYPE_SAMPLER;
            break;
        case DescriptorType::STORAGE_IMAGE:
            vkType = VkDescriptorType::VK_DESCRIPTOR_TYPE_SAMPLER;
            break;
        case DescriptorType::UNIFORM_TEXEL_BUFFER:
            vkType = VkDescriptorType::VK_DESCRIPTOR_TYPE_SAMPLER;
            break;
        case DescriptorType::STORAGE_TEXEL_BUFFER:
            vkType = VkDescriptorType::VK_DESCRIPTOR_TYPE_SAMPLER;
            break;
        case DescriptorType::UNIFORM_BUFFER:
            vkType = VkDescriptorType::VK_DESCRIPTOR_TYPE_SAMPLER;
            break;
        case DescriptorType::STORAGE_BUFFER:
            vkType = VkDescriptorType::VK_DESCRIPTOR_TYPE_SAMPLER;
            break;
        case DescriptorType::UNIFORM_BUFFER_DYNAMIC:
            vkType = VkDescriptorType::VK_DESCRIPTOR_TYPE_SAMPLER;
            break;
        case DescriptorType::STORAGE_BUFFER_DYNAMIC:
            vkType = VkDescriptorType::VK_DESCRIPTOR_TYPE_SAMPLER;
            break;
        case DescriptorType::INPUT_ATTACHMENT:
            vkType = VkDescriptorType::VK_DESCRIPTOR_TYPE_SAMPLER;
            break;
    }

    return vkType;
}

ShaderType VkShaderResourceManager::GetTypeFromName(const std::string & shaderName)
{
    ShaderType type;
    if (shaderName.find("frag") != std::string::npos)
        type = ShaderType::FRAGMENT;
    else if (shaderName.find("vert") != std::string::npos)
        type = ShaderType::VERTEX;

    return type;
}

std::string VkShaderResourceManager::GetShaderNameFromRefl(const std::string & reflName)
{
    std::string shaderName = reflName.substr(0, reflName.find_last_of("."));

    shaderName = shaderName.substr(reflName.find_last_of("\\")+1);
    return shaderName;
}

void VkShaderResourceManager::Init()
{
    std::vector<std::string> fileList;
    GetFilesInFolder(VULKAN_ASSETS_PATH + std::string{ "/Reflections" }, fileList);

    uint32_t numUniform = 0;

    for each (std::string var in fileList)
    {
        // load the json file
        size_t fileLength = 0;
        const char* reflString = ReadTxtFile((uint32_t&)fileLength, var.c_str());

        Document reflDoc;
        reflDoc.Parse(reflString, fileLength);
        if (reflDoc.HasParseError())
            ASSERT_MSG(0, "Error parsing material file");
        free((void*)reflString);

        //TODO : Include push constants 
        if (reflDoc.HasMember("push_constants"))
        {
            const Value& pushConstants = reflDoc["push_constants"];

            const Value& elements = pushConstants["elements"];
            if (!elements.IsArray())
                printf("push constant block in reflection file does not contain a member array\n");

            for (SizeType elem = 0; elem < elements.Size(); elem++)
            {
                const Value& element = elements[elem];
                if (element["name"].GetStringLength() > 31)
                    ASSERT_MSG(0, "opaque block member names must be less than 32 characters");

                UniformStructMember mem;
                snprintf(mem.name, sizeof(mem.name), "%s", element["name"].GetString());
                mem.offset = element["offset"].GetInt();
                mem.size = element["size"].GetInt();
            }
        }

        bool hasDescriptors = reflDoc.HasMember("descriptor_sets");
        if (hasDescriptors)
        {
            const Value& reflFileDescSets = reflDoc["descriptor_sets"];
            numUniform = reflFileDescSets.Size();
        }
        else
            numUniform = 0;

        if (numUniform > 0)
        {
            const Value& reflFileDescSets = reflDoc["descriptor_sets"];

            std::vector<BindingWrapper> bindingList;
            int currentSet = -1;

            std::map<uint32_t, bool> setCreatedMap;

            for (SizeType dsIdx = 0; dsIdx < reflFileDescSets.Size(); dsIdx++)
            {
                const Value& currentInputFromReflData = reflFileDescSets[dsIdx];

                // this logic works only if the sets are kept in increasing order and 
                // they are not repeating in a given shader, they can have multiple bindings.
                int set = currentInputFromReflData["set"].GetInt();

                if (currentSet == -1)
                    currentSet = set;

                if (currentSet != set )
                {
                    setCreatedMap.insert(std::pair<uint32_t, bool>({ currentSet, true }));
                    // create the set using previously accumulated bindings
                    CreateUniqueSetLayoutWrapper(bindingList, var, currentSet);

                    // clear the binding list
                    bindingList.clear();

                    // change the current set
                    currentSet = set;
                }

                // add binding to the binding list

                BindingWrapper wrapper = {};
                //wrapper.id = GetBindingID();
                wrapper.bindingObj.binding = currentInputFromReflData["binding"].GetInt();
                
                int arraySize = currentInputFromReflData["ArraySize"].GetInt();
                if(arraySize == 0)
                    wrapper.bindingObj.descriptorCount = 1;
                else
                    wrapper.bindingObj.descriptorCount = arraySize;

                wrapper.bindingObj.descriptorType = StringToDescType(currentInputFromReflData["type"].GetString());

                //add all members to block definition
                const Value& reflBlockMembers = currentInputFromReflData["members"];
                for (SizeType m = 0; m < reflBlockMembers.Size(); m++)
                {
                    const Value& reflBlockMember = reflBlockMembers[m];

                    UniformStructMember mem = {};
                    mem.offset = reflBlockMember["offset"].GetInt();
                    mem.size = reflBlockMember["size"].GetInt();

                    snprintf(mem.name, sizeof(mem.name), "%s", reflBlockMember["name"].GetString());

                    wrapper.memberList.push_back(mem);
                }

                bindingList.push_back(wrapper);
                
                if (dsIdx == reflFileDescSets.Size() - 1 && setCreatedMap.find(currentSet) == setCreatedMap.end())
                {
                    // create the set using previously accumulated bindings
                    CreateUniqueSetLayoutWrapper(bindingList, var, currentSet);
                }

                continue;

                ShaderType shaderType;
                if (var.find("frag") != std::string::npos)
                    shaderType = ShaderType::FRAGMENT;
                else if(var.find("vert") != std::string::npos)
                    shaderType = ShaderType::VERTEX;

                // DONT DELETE
                /*
                res.uniformDef[dsIdx].uniformSize = GetGPUAlignedSize(currentInputFromReflData["size"].GetInt());
                res.uniformDef[dsIdx].set = currentInputFromReflData["set"].GetInt();
                res.uniformDef[dsIdx].binding = currentInputFromReflData["binding"].GetInt();
                res.uniformDef[dsIdx].type = StringToDescType(currentInputFromReflData["type"].GetString());
                res.uniformDef[dsIdx].uniformName = currentInputFromReflData["name"].GetString();
                res.uniformDef[dsIdx].shaderType = shaderType;
                res.uniformDef[dsIdx].shaderName = var;

                BindingWrapper wrapper = {};
                wrapper.id = GetBindingID();
                wrapper.bindingObj.binding = currentInputFromReflData["binding"].GetInt();
                wrapper.bindingObj.descriptorCount = currentInputFromReflData["ArraySize"].GetInt();
                wrapper.bindingObj.descriptorType = StringToDescType(currentInputFromReflData["type"].GetString());
                */
            }
        }
        // shader doesn't has any descriptor sets
        else
        {
            ShaderResources obj = {};
            obj.shaderName = GetShaderNameFromRefl(var);
            perShaderResourceList.push_back(obj);
        }
    }
}

void VkShaderResourceManager::DeInit()
{
    for (uint32_t i = 0; i < setWrapperList.size(); i++)
    {
        delete setWrapperList[i];
    }

    setWrapperList.clear();
    perShaderResourceList.clear();
}

void VkShaderResourceManager::Update()
{

}

VkShaderResourceManager * VkShaderResourceManager::GetInstance()
{
    if (instance == nullptr)
    {
        instance = new VkShaderResourceManager();
    }
    return instance;
}

VkShaderResourceManager::~VkShaderResourceManager()
{
}

std::vector<SetWrapper*> VkShaderResourceManager::GetSetsForShaders(const std::vector<std::string>& shaderNames)
{
    std::vector<SetWrapper*> setList;

    for each (std::string name in shaderNames)
    {
        std::vector<ShaderResources>::iterator it;
        it = std::find_if(perShaderResourceList.begin(), perShaderResourceList.end(), [&](ShaderResources e) { return e.shaderName == name; });

        if (it == perShaderResourceList.end())
        {
            ASSERT_MSG(0, "shader name mismatch");
        }
        else if (it->setWrappers.size() == 0)
            continue;
        else
        {
            for(uint32_t i = 0; i < it->setWrappers.size(); i++)
            {
                SetWrapper* obj = it->setWrappers[i];
                std::vector<SetWrapper*>::iterator itt;
                itt = std::find_if(setList.begin(), setList.end(), [&](SetWrapper* e) { return e->id == obj->id; });

                if(itt == setList.end())
                    setList.push_back(it->setWrappers[i]);
            }
        }
    }

    return setList;
}
