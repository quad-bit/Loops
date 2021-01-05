#pragma once
#include "Material.h"
#include "Shader.h"
#include <map>

struct ShaderDescription;

class MaterialFactory
{
private:
    MaterialFactory(){}
    MaterialFactory(MaterialFactory const &) {}
    MaterialFactory const & operator= (MaterialFactory const &) {}

    static MaterialFactory* instance;

    uint32_t matIdCounter = 0;
    uint32_t GetMatId();
    void DecrementMatCounter();

    std::map<uint32_t, Material*> idToMaterialMap;

public:
    void Init();
    void DeInit();
    void Update();
    static MaterialFactory* GetInstance();
    ~MaterialFactory();

    //Material * CreateMaterial(Shader * shaders, const uint32_t & shaderCount, const uint32_t & meshId);
    Material * CreateMaterial(ShaderDescription * shaders, const uint32_t & shaderCount, const uint32_t & meshId);
};
