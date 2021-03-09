#include "VkShaderResourceManager.h"
#include "FileReader.h"
#include <stdio.h>
#include "VulkanUtility.h"
#include "VulkanManager.h"
#include <Assertion.h>
#include <HashManager.h>
#include <VkRenderingUnwrapper.h>
#include "VkShaderResourceAllocator.h"
#include "VkDescriptorPoolFactory.h"
#include "VkBufferFactory.h"

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

    ASSERT_MSG_DEBUG(0 , "trying to convert an invalid string to input type");
    return DescriptorType::NUM_TYPES;
}

uint32_t VkShaderResourceManager::GetBindingID()
{
    return bindingIdCounter++;
}

uint32_t VkShaderResourceManager::GetSetLayoutID()
{
    return setWrapperIdCounter++;
}

uint32_t VkShaderResourceManager::GetPipelineLayoutID()
{
    return pipelineLayoutIdCounter++;
}

uint32_t VkShaderResourceManager::GetVkDescriptorSetLayoutWrapperID()
{
    return setLayoutWrapperIdCounter++;
}

uint32_t VkShaderResourceManager::GetVkDescriptorSetID()
{
    return vkdescriptorSetIdCounter++;
}

void VkShaderResourceManager::CreateUniqueSetLayoutWrapper(std::vector<BindingWrapper> & bindingList, std::string shaderName, uint32_t set)
{
    std::string glslShader = GetShaderNameFromRefl(shaderName);

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
            ASSERT_MSG_DEBUG(0, "Id mismatch");
        }

        (*it)->shaderNames.push_back(glslShader);
        (*it)->shaderFlags.push_back(GetTypeFromName(glslShader));

        DecrementSetLayoutId();

        AccumulateSetLayoutPerShader(glslShader, (*it));

        delete setWrapper;
    }
    else
    {
        // if not add this new object to the wrapper list
        setWrapper->shaderNames.push_back(glslShader);
        setWrapper->shaderFlags.push_back(GetTypeFromName(glslShader));
        setWrapperList.push_back(setWrapper);
        
        AccumulateSetLayoutPerShader(glslShader, setWrapper);

        // create vulkan descriptor set layout per set wrapper
        {
            VkDescriptorSetLayout * setLayout = UnwrapSetwrapper(setWrapper);
            uint32_t id = GetVkDescriptorSetLayoutWrapperID();
            idToSetLayoutMap.insert(std::pair<uint32_t, VkDescriptorSetLayout *>(id, setLayout));
            setWrapper->descriptorSetLayoutId = id;
        }

        // Create per scene descriptor sets
        {
            /*uint32_t id = GetVkDescriptorSetID();
            VkDescriptorSet * set = resourceAllocator->AllocateDescriptor(setWrapper);
            setWrapper->descriptorSetId = id;

            idToSetMap.insert(std::pair<uint32_t, VkDescriptorSet *>(id, set));
            */
        }
    }
}

void VkShaderResourceManager::AccumulateSetLayoutPerShader(std::string glslName, SetWrapper * setWrapper)
{
    //std::string glslShader = GetShaderNameFromRefl(reflName);

    std::vector<ShaderResources>::iterator it;
    it = std::find_if(perShaderResourceList.begin(), perShaderResourceList.end(), [&](ShaderResources e) { return e.shaderName == glslName; });

    if (it == perShaderResourceList.end())
    {
        ShaderResources obj = {};
        obj.shaderName = glslName;
        obj.setWrappers.push_back(setWrapper);
        perShaderResourceList.push_back(obj);
    }
    else
    {
        it->setWrappers.push_back(setWrapper);
    }
}


VkDescriptorSetLayout * VkShaderResourceManager::UnwrapSetwrapper(SetWrapper * setWrapper)
{
    std::vector<VkDescriptorSetLayoutBinding> bindingList;
    uint32_t numBindings = (uint32_t)setWrapper->bindingWrapperList.size();
    bindingList.resize(numBindings);

    for (uint32_t i = 0; i < numBindings; i++)
    {
       DescriptorSetLayoutBinding obj = setWrapper->bindingWrapperList[i].bindingObj;
       bindingList[i].binding = obj.binding;
       bindingList[i].descriptorCount = obj.descriptorCount;
       bindingList[i].descriptorType = UnwrapDescriptorType(obj.descriptorType);
       bindingList[i].stageFlags = VulkanUnwrap::UnwrapShaderStage(obj.stageFlags.data(), (uint32_t)obj.stageFlags.size());
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
            vkType = VkDescriptorType::VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
            break;
        case DescriptorType::SAMPLED_IMAGE:
            vkType = VkDescriptorType::VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE;
            break;
        case DescriptorType::STORAGE_IMAGE:
            vkType = VkDescriptorType::VK_DESCRIPTOR_TYPE_STORAGE_IMAGE;
            break;
        case DescriptorType::UNIFORM_TEXEL_BUFFER:
            vkType = VkDescriptorType::VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER;
            break;
        case DescriptorType::STORAGE_TEXEL_BUFFER:
            vkType = VkDescriptorType::VK_DESCRIPTOR_TYPE_STORAGE_TEXEL_BUFFER;
            break;
        case DescriptorType::UNIFORM_BUFFER:
            vkType = VkDescriptorType::VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
            break;
        case DescriptorType::STORAGE_BUFFER:
            vkType = VkDescriptorType::VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
            break;
        case DescriptorType::UNIFORM_BUFFER_DYNAMIC:
            vkType = VkDescriptorType::VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC;
            break;
        case DescriptorType::STORAGE_BUFFER_DYNAMIC:
            vkType = VkDescriptorType::VK_DESCRIPTOR_TYPE_STORAGE_BUFFER_DYNAMIC;
            break;
        case DescriptorType::INPUT_ATTACHMENT:
            vkType = VkDescriptorType::VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT;
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

VkDescriptorSet VkShaderResourceManager::GetDescriptorSet(const uint32_t & id)
{
    return idToSetMap[id];
}


void VkShaderResourceManager::Init()
{
    //resourceAllocator = new VkShaderResourceAllocator();

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
            ASSERT_MSG_DEBUG(0, "Error parsing material file");
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
                    ASSERT_MSG_DEBUG(0, "opaque block member names must be less than 32 characters");

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
                wrapper.bindingName = currentInputFromReflData["name"].GetString();
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

                ShaderType shaderType;
                if (var.find("frag") != std::string::npos)
                    shaderType = ShaderType::FRAGMENT;
                else if (var.find("vert") != std::string::npos)
                    shaderType = ShaderType::VERTEX;
                wrapper.bindingObj.stageFlags.push_back(shaderType);

                bindingList.push_back(wrapper);
                
                if (dsIdx == reflFileDescSets.Size() - 1 && setCreatedMap.find(currentSet) == setCreatedMap.end())
                {
                    // create the set using previously accumulated bindings
                    CreateUniqueSetLayoutWrapper(bindingList, var, currentSet);
                }

                continue;

                // DONT DELETE
                /*
                ShaderType shaderType;
                if (var.find("frag") != std::string::npos)
                    shaderType = ShaderType::FRAGMENT;
                else if(var.find("vert") != std::string::npos)
                    shaderType = ShaderType::VERTEX;

                
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

    // Create filler set
    VkDescriptorSetLayoutCreateInfo info = {};
    info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
    info.bindingCount = 0;
    info.pBindings = nullptr;

    ErrorCheck(vkCreateDescriptorSetLayout(
        *CoreObjects::logicalDeviceObj,
        &info,
        CoreObjects::pAllocator,
        &fillerSetLayout
    ));

    VkDescriptorSetAllocateInfo allocInfo = {};
    allocInfo.descriptorPool = *VkDescriptorPoolFactory::GetInstance()->GetDescriptorPool();
    allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
    allocInfo.descriptorSetCount = 1;
    allocInfo.pSetLayouts = &fillerSetLayout;

    ErrorCheck(vkAllocateDescriptorSets(*CoreObjects::logicalDeviceObj, &allocInfo, &fillerSet));
}

void VkShaderResourceManager::DeInit()
{
    //delete resourceAllocator;

    for (uint32_t i = 0; i < setWrapperList.size(); i++)
    {
        delete setWrapperList[i];
    }

    setWrapperList.clear();
    perShaderResourceList.clear();
    pipelineLayoutIdToSetValuesMap.clear();

    for (uint32_t i = 0; i < (uint32_t)pipelineLayoutWrapperList.size(); i++)
    {
        
        vkDestroyPipelineLayout(*CoreObjects::logicalDeviceObj, *pipelineLayoutWrapperList[i].pipelineLayout, CoreObjects::pAllocator);
        delete pipelineLayoutWrapperList[i].pipelineLayout;
    }

    for (auto const& obj : idToSetLayoutMap)
    {
        vkDestroyDescriptorSetLayout(*CoreObjects::logicalDeviceObj, *obj.second, CoreObjects::pAllocator);
        delete obj.second;
    }

    idToSetLayoutMap.clear();

    for (auto const& obj : fillerSetLayouts)
    {
        vkDestroyDescriptorSetLayout(*CoreObjects::logicalDeviceObj, obj, CoreObjects::pAllocator);
    }

    fillerSetLayouts.clear();
    vkDestroyDescriptorSetLayout(*CoreObjects::logicalDeviceObj, fillerSetLayout, CoreObjects::pAllocator);

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
            ASSERT_MSG_DEBUG(0, "shader name mismatch");
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

uint32_t VkShaderResourceManager::CreatePipelineLayout(SetWrapper ** setWrapperList, const size_t & numSets)
{
    PipelineLayoutWrapper pipelineLayoutWrapperObj = {};
    pipelineLayoutWrapperObj.id = GetPipelineLayoutID();
    //pipelineLayoutWrapperObj.setLayoutList.resize(numSets);

    GetSetLayouts(setWrapperList, (uint32_t)numSets, pipelineLayoutWrapperObj.setLayoutList, pipelineLayoutWrapperObj.setValuesInPipelineLayout);
    
    VkPipelineLayoutCreateInfo pipelineCreateInfo = {};
    pipelineCreateInfo.flags = 0;
    pipelineCreateInfo.pPushConstantRanges= nullptr;
    pipelineCreateInfo.pSetLayouts = pipelineLayoutWrapperObj.setLayoutList.data();
    pipelineCreateInfo.pNext = nullptr;
    pipelineCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
    pipelineCreateInfo.pushConstantRangeCount = 0;
    pipelineCreateInfo.setLayoutCount = (uint32_t)pipelineLayoutWrapperObj.setLayoutList.size();

    VkPipelineLayout * pipelineLayout = new VkPipelineLayout;
    ErrorCheck(vkCreatePipelineLayout(*CoreObjects::logicalDeviceObj, &pipelineCreateInfo, CoreObjects::pAllocator, pipelineLayout));

    pipelineLayoutWrapperObj.pipelineLayout = pipelineLayout;
    pipelineLayoutWrapperList.push_back(pipelineLayoutWrapperObj);
    
    pipelineLayoutIdToSetValuesMap.insert(std::pair<uint32_t, std::vector<int>>(
    { pipelineLayoutWrapperObj.id, pipelineLayoutWrapperObj.setValuesInPipelineLayout}));

    return pipelineLayoutWrapperObj.id;
}

VkPipelineLayout * VkShaderResourceManager::GetPipelineLayout(const uint32_t & id)
{
    std::vector<PipelineLayoutWrapper>::iterator it;

    it = std::find_if(pipelineLayoutWrapperList.begin(), pipelineLayoutWrapperList.end(), [&](PipelineLayoutWrapper e) {
        return e.id == id; });
    
    ASSERT_MSG_DEBUG(it != pipelineLayoutWrapperList.end(), "id not found");
    
    return (it)->pipelineLayout;
}

std::vector<SetWrapper*>* VkShaderResourceManager::GetSetWrapperList()
{
    return &setWrapperList;
}

void VkShaderResourceManager::GetSetLayouts(SetWrapper ** setWrapperList, const uint32_t & numSets, std::vector<VkDescriptorSetLayout> & layoutList, std::vector<int> & setValueList)
{
    // order of sets (descriptor types) should match order mentioned in shaders 

    std::vector<uint32_t> setValues;
    for (uint32_t i = 0; i < numSets; i++)
    {
        SetWrapper * wrapper = setWrapperList[i];
        setValues.push_back(wrapper->setValue);
    }

    std::sort(setValues.begin(), setValues.end());
    std::vector<uint32_t> completeSet;
    {
        for (uint32_t i = setValues.front(); i <= setValues.back(); i++)
        {
            completeSet.push_back(i);
        }

        for each(auto obj in completeSet)
        {
            std::vector<uint32_t>::iterator it;
            it = std::find_if(setValues.begin(), setValues.end(), [&](uint32_t e) {
                return e == obj; });

            // if the setvalue is present in setwrapper add the existing setlayout
            if (it != setValues.end())
            {
                for (uint32_t i = 0; i < numSets; i++)
                {
                    SetWrapper * wrapper = setWrapperList[i];
                    if (wrapper->setValue == obj)
                    {
                        layoutList.push_back(*idToSetLayoutMap[wrapper->descriptorSetLayoutId]);
                        setValueList.push_back(obj);
                        break;
                    }
                }
            }
            // if not create a dummy set layout and add it to the list
            else
            {
                //VkDescriptorSetLayoutCreateInfo info = {};
                //info.bindingCount = 0;
                //info.pBindings = nullptr;
                //info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;

                //VkDescriptorSetLayout setLayout;
                //ErrorCheck(vkCreateDescriptorSetLayout(
                //    *CoreObjects::logicalDeviceObj,
                //    &info,
                //    CoreObjects::pAllocator,
                //    &setLayout
                //));
                layoutList.push_back(fillerSetLayout);
                //fillerSetLayouts.push_back(setLayout);
                setValueList.push_back(-1);
            }
        }
    }

    return;
    /*
    uint32_t setValue = 0; 

    for (uint32_t i = 0; i < (uint32_t)setValues.size(); i++)
    {
        uint32_t diff = setValues[i] - setValue;
        if (diff > 1)
        {
            // Create new diff * layouts
            for (uint32_t i = 0; i < diff - 1; i++)
            {
                VkDescriptorSetLayoutBinding obj = {};
                obj.binding = 0;
                obj.descriptorCount = 0;
                obj.descriptorType = VkDescriptorType::VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
                obj.pImmutableSamplers = nullptr;
                obj.stageFlags = VkShaderStageFlagBits::VK_SHADER_STAGE_VERTEX_BIT;

                VkDescriptorSetLayoutCreateInfo info = {};
                info.bindingCount = 0;
                info.pBindings = nullptr;
                info.sType= VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;

                VkDescriptorSetLayout setLayout;
                ErrorCheck(vkCreateDescriptorSetLayout(
                    *CoreObjects::logicalDeviceObj,
                    &info,
                    CoreObjects::pAllocator,
                    &setLayout
                ));
                layoutList.push_back(setLayout);
                fillerSetLayouts.push_back(setLayout);
                //setValue += 1;
            }
        }
//        else
        {
            for (uint32_t i = 0; i < numSets; i++)
            {
                SetWrapper * wrapper = setWrapperList[i];

                if (idToSetLayoutMap.find(wrapper->descriptorSetLayoutId) == idToSetLayoutMap.end())
                {
                    ASSERT_MSG(0, "Id mismatch");
                }
                else if(setValues[i] == wrapper->setValue)
                {
                    layoutList.push_back(*idToSetLayoutMap[wrapper->descriptorSetLayoutId]);
                    break;
                }
            }
            setValue = setValues[i];
        }
    }
    */
}

std::vector<VkDescriptorSet> VkShaderResourceManager::GetDescriptors(uint32_t * ids, const uint32_t & count, const uint32_t & pipelineLayoutId)
{
    std::vector<int> * setValues = &pipelineLayoutIdToSetValuesMap[pipelineLayoutId];
    std::vector<VkDescriptorSet> list;

    for (uint32_t i = 0, k = 0; i < setValues->size(); i++)
    {
        if (setValues->at(i) != -1)
        {
            VkDescriptorSet set = idToSetMap[ids[k]];
            ASSERT_MSG_DEBUG(set != VK_NULL_HANDLE, "Set not found");
            list.push_back(set);
            k++;
        }
        else
        {
            list.push_back(fillerSet);
        }
    }
    return list;
}

std::tuple<std::vector<VkDescriptorSet>, uint32_t> VkShaderResourceManager::GetDescriptors(uint32_t * ids, const uint32_t & count, const uint32_t & pipelineLayoutId, const uint32_t & firstSet)
{
    std::vector<int> * setValues = &pipelineLayoutIdToSetValuesMap[pipelineLayoutId];
    std::vector<VkDescriptorSet> list;
    uint32_t offset;
    for (uint32_t i = 0, k = 0; i < setValues->size(); i++)
    {
        if (setValues->at(i) != -1)
        {
            if (firstSet == k)
                offset = i;
            VkDescriptorSet set = idToSetMap[ids[k]];
            ASSERT_MSG_DEBUG(set != VK_NULL_HANDLE, "Set not found");
            list.push_back(set);
            k++;
        }
        else
        {
            list.push_back(fillerSet);
        }
    }

    return std::make_tuple(list, offset);
}

uint32_t * VkShaderResourceManager::AllocateDescriptors(SetWrapper * setwrapper, const uint32_t & numDescriptors)
{
    uint32_t * ids = new uint32_t[numDescriptors];
    VkDescriptorPool * pool = VkDescriptorPoolFactory::GetInstance()->GetDescriptorPool();
    VkDescriptorSetLayout * layout = idToSetLayoutMap[setwrapper->descriptorSetLayoutId];

    std::vector<VkDescriptorSetLayout> setLayouts;
    std::vector<VkDescriptorSet> sets;
    sets.resize(numDescriptors);

    for (uint32_t i = 0; i < numDescriptors; i++)
    {
        ids[i] = GetVkDescriptorSetID();
        setLayouts.push_back(*layout);
    }

    VkDescriptorSetAllocateInfo allocInfo = {};
    allocInfo.descriptorPool = *pool;
    allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
    allocInfo.descriptorSetCount = numDescriptors;
    allocInfo.pSetLayouts = setLayouts.data();

    ErrorCheck( vkAllocateDescriptorSets(*CoreObjects::logicalDeviceObj, &allocInfo, sets.data()));
    
    for (uint32_t i = 0; i < numDescriptors; i++)
        idToSetMap.insert(std::pair<uint32_t, VkDescriptorSet>(ids[i], sets[i]));

    return ids;
}

void VkShaderResourceManager::LinkSetBindingToResources(ShaderBindingDescription * desc)
{
    VkDescriptorType descriptorType = UnwrapDescriptorType(desc->resourceType);

    uint32_t numWrites = (uint32_t)desc->descriptorIds.size();
    uint32_t numDescriptors = numWrites;

    std::vector<VkDescriptorBufferInfo> bufferInfoList;
    std::vector<VkDescriptorImageInfo> imageInfoList;

    switch (descriptorType)
    {
    case VkDescriptorType::VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER:
        for (uint32_t i = 0; i < numWrites; i++)
        {
            VkDescriptorBufferInfo info = {};
            info.buffer = *VkBufferFactory::GetInstance()->GetBuffer(desc->resourceId);
            info.offset = desc->offsetsForEachDescriptor[i];
            info.range = desc->dataSizePerDescriptorAligned;
            bufferInfoList.push_back(info);
        }
        break;

    case VkDescriptorType::VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER:
        ASSERT_MSG_DEBUG(0, "Yet to be implemented");
        break;

    case VkDescriptorType::VK_DESCRIPTOR_TYPE_SAMPLER:
        ASSERT_MSG_DEBUG(0, "Yet to be implemented");
        break;

    default:
        ASSERT_MSG_DEBUG(0, "Yet to be implemented");
    }

    std::vector<VkWriteDescriptorSet> writeList;

    for (uint32_t i = 0; i < numWrites; i++)
    {
        VkWriteDescriptorSet write = {};
        write.descriptorCount = 1; //since linking only one binding of a set
        write.descriptorType = descriptorType;
        write.dstArrayElement = 0;
        write.dstBinding = desc->binding;
        write.pBufferInfo = &bufferInfoList[i];
        write.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
        write.dstSet = GetDescriptorSet(desc->descriptorIds[i]);
        writeList.push_back(write);
    }

    vkUpdateDescriptorSets(*CoreObjects::logicalDeviceObj, (uint32_t)writeList.size(), writeList.data(), 0, nullptr);
}

const std::vector<int>* VkShaderResourceManager::GetSetValuesInPipelineLayout(const uint32_t & pipelineLayoutId)
{
    return &pipelineLayoutIdToSetValuesMap[pipelineLayoutId];
}
