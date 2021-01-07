#include "MaterialFactory.h"
#include "HashManager.h"
#include "Assertion.h"
#include "ShaderFactory.h"
#include "RenderingWrapper.h"

MaterialFactory* MaterialFactory::instance = nullptr;

uint32_t MaterialFactory::GetMatId()
{
    return matIdCounter++;
}

void MaterialFactory::DecrementMatCounter()
{
    matIdCounter--;
}

void MaterialFactory::CreateSetResources(std::vector<SetWrapper*> setWrapperList)
{
    // Iterate through all the wrappers check if the corresponding resources have been 
    // created for each set
}

void MaterialFactory::Init()
{

}

void MaterialFactory::DeInit()
{
    for each (auto const& var in idToMaterialMap)
    {
        delete var.second;
    }
    idToMaterialMap.clear();
}

void MaterialFactory::Update()
{

}

MaterialFactory * MaterialFactory::GetInstance()
{
    if (instance == nullptr)
    {
        instance = new MaterialFactory();
    }
    return instance;
}
          
MaterialFactory::~MaterialFactory()
{
}

Material * MaterialFactory::CreateMaterial(ShaderDescription * shaderDescriptions, const uint32_t & shaderCount, const uint32_t & meshId)
{
    std::vector<std::string> shaderNames;
    shaderNames.resize(shaderCount);

    for (uint32_t i = 0; i < shaderCount; i++)
    {
        shaderNames[i] = shaderDescriptions[i].shaderName;
    }
    
    uint32_t matId = GetMatId();

    // check for hash (shader names)
    int id = HashManager::GetInstance()->FindMaterialHash(&shaderNames, matId);
    Material * mat;

    if (id == -1)
    {
        // use matid and create a material

        Shader * shaders = new Shader[shaderCount];
        for (uint32_t i = 0; i < shaderCount; i++)
        {
            shaders[i].shaderType = shaderDescriptions[i].type;
            shaders[i].shaderName = shaderDescriptions[i].shaderName;
        }
        
        std::vector<SetWrapper*> setWrapperList =  ShaderFactory::GetInstance()->CreateShader(meshId, shaders, shaderCount);
        
        mat = new Material(shaders, shaderCount);
        idToMaterialMap.insert(std::pair<uint32_t, Material*>(
            { matId, mat }));
        
        // get the setWrappers for the shader combination
        mat->resourceLayoutList = setWrapperList;

        // using the set wrappers create shader resources
        CreateSetResources(setWrapperList);
        

    }
    else
    {
        if (idToMaterialMap.find(id) != idToMaterialMap.end())
        {
            mat = idToMaterialMap[id];
        }
        else
        {
            ASSERT_MSG(0, "Mat not found");
        }
        DecrementMatCounter();

        // if it exists then add the meshid to the list
        ShaderFactory::GetInstance()->AddMeshToShader(meshId, mat->shaders, shaderCount);
    }


    return mat;
}
