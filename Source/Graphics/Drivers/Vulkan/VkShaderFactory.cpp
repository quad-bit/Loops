#include "VkShaderFactory.h"
#include <iostream>
#include <FileReader.h>
#include <RenderingWrapper.h>
#include <Assertion.h>
#include "VulkanUtility.h"
#include "VulkanSettings.h"

VkShaderFactory* VkShaderFactory::instance = nullptr;

void VkShaderFactory::LoadSpv()
{
}

uint32_t VkShaderFactory::AssignId()
{
    return idCounter++;
}

VkShaderModule * VkShaderFactory::CreateShaderModule(uint32_t * shaderCode, size_t codeLength)
{
    VkShaderModule * shaderModuleObj = new VkShaderModule;

    VkShaderModuleCreateInfo createInfo{};
    createInfo.codeSize = codeLength;
    createInfo.pCode = shaderCode;
    createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;

    ErrorCheck(vkCreateShaderModule(*CoreObjects::logicalDeviceObj, &createInfo, CoreObjects::pAllocator, shaderModuleObj));

    return shaderModuleObj;
}

void VkShaderFactory::GetSpvNameList(const std::vector<std::string> & fileNameList)
{
    for (const auto & entry : fileNameList)
    {
        if( entry.find(".spv") != std::string::npos)
            spvNames.push_back(entry);
    }
}

std::string VkShaderFactory::GetSpvName(const std::string & shaderName)
{
    std::string fileNameWithoutExt;
    size_t lastindex = shaderName.find_last_of(".");
    fileNameWithoutExt = shaderName.substr(0, lastindex);

    if (shaderName.find(".frag") != std::string::npos)
    {
        return fileNameWithoutExt +"Frag.spv";
    }
    else if (shaderName.find(".vert") != std::string::npos)
    {
        return fileNameWithoutExt +"Vert.spv";
    }
    
    ASSERT_MSG(0, "Invalid shader name");
    return "NULL";
}

void VkShaderFactory::Init()
{
    std::vector<std::string> fileList;
    GetFilesInFolder(VULKAN_ASSETS_PATH + std::string{ "/shaders" }, fileList);
    
    for each (std::string var in fileList)
    {
        //std::cout << var << std::endl;
        if (var.find(".spv") == std::string::npos)
            continue;

        // load the text
        size_t fileLength = 0;
        const char* shaderText = ReadSpvFile((uint32_t&)fileLength, var.c_str());
        //const char* shaderText = ReadFile(fileLength, var.c_str());
        
        ShaderModuleWrapper * wrapper = new ShaderModuleWrapper;

        // if path contains frag then it fragment shader
        if (var.find("Frag") != std::string::npos)
        {
            wrapper->shaderType = new ShaderType{ ShaderType::FRAGMENT };
        }
        else if (var.find("Vert") != std::string::npos)
        {
            wrapper->shaderType = new ShaderType{ ShaderType::VERTEX };
        }
        
        wrapper->shaderName = var;
        wrapper->shaderId = AssignId();
        wrapper->module = CreateShaderModule((uint32_t*)shaderText, (size_t)fileLength );

        // add the module to list
        shaderModuleList.push_back(wrapper);
    }
}

void VkShaderFactory::DeInit()
{
    for each (ShaderModuleWrapper* var in shaderModuleList)
    {
        vkDestroyShaderModule(*CoreObjects::logicalDeviceObj, *var->module, CoreObjects::pAllocator);
        delete var->module;
        delete var->shaderType;
        delete var;
    }

    shaderModuleList.clear();
}

void VkShaderFactory::Update()
{

}

VkShaderFactory * VkShaderFactory::GetInstance()
{
    if (instance == nullptr)
    {
        instance = new VkShaderFactory();
    }
    return instance;
}

VkShaderFactory::~VkShaderFactory()
{
}

void VkShaderFactory::GetShaderIds( char ** shaderName, ShaderType * type, uint32_t * id, const uint32_t & shaderCount)
{
    std::vector<ShaderModuleWrapper *>::iterator it;

    for (uint32_t i = 0; i < shaderCount; i++)
    {
        string spvName = GetSpvName(shaderName[i]);
        it = std::find_if(shaderModuleList.begin(), shaderModuleList.end(), [&](ShaderModuleWrapper * wrapper) { 
            return ( wrapper->shaderName.find (spvName) != std::string::npos &&
                 *wrapper->shaderType == type[i]); });

        ASSERT_MSG(it != shaderModuleList.end(), "Shader not found");
        id[i] = (*it)->shaderId;
    }
}
