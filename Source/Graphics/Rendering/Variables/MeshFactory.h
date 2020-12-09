#pragma once
#include "BitArray.h"
#include <map>

enum class MESH_TYPE;
class Mesh;
struct WrapperBase;

#if (RENDERING_API == VULKAN)
class VulkanInterface;
#elif (RENDERING_API == DX)
class DxInterface;
#endif

class MeshFactory
{
private:

#if (RENDERING_API == VULKAN)
    VulkanInterface * apiInterface;
#elif (RENDERING_API == DX)
    DxInterface * apiInterface;
#endif

    MeshFactory(){}
    MeshFactory(MeshFactory const &) {}
    MeshFactory const & operator= (MeshFactory const &) {}

    static MeshFactory* instance;
    std::map<Mesh *, WrapperBase *> meshToVertWrapperMap;

    uint32_t idCounter = 0;
    uint32_t GetId() { return idCounter++; }

    int * CreateVertexBuffer(uint32_t count);
    int CreateIndexBuffer();
    //Create vertex input location description
    //Create vertex input binding description

public:
#if (RENDERING_API == VULKAN)
    void Init(VulkanInterface * obj) { apiInterface = obj; }
#elif (RENDERING_API == DX)
    void Init(DxInterface * obj) { apiInterface = obj; }
#endif

    void DeInit();
    void Update();
    static MeshFactory* GetInstance();
    ~MeshFactory();

    Mesh * CreateMesh(const BitArray & attribMaskReq, MESH_TYPE * meshType, bool isIndexed, bool individualBuffersRequired);
    void DestroyMesh(const uint32_t & meshId);
};
