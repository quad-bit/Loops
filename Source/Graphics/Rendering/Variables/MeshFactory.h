#pragma once
#include "BitArray.h"
#include <map>
#include "PlatformSettings.h"
#include <bitset>
#include <AttributeHelper.h>

enum class MESH_TYPE;
class Mesh;
struct AttribStructBase;
struct MeshInfo;

#if (RENDERING_API == VULKAN)
    class VulkanInterface;
    typedef VulkanInterface ApiInterface;
#elif (RENDERING_API == DX)
    class DxInterface;
    typedef DxInterface ApiInterface;
#endif

class MeshFactory
{
private:
    ApiInterface * apiInterface;
    MeshFactory(){}
    MeshFactory(MeshFactory const &) {}
    MeshFactory const & operator= (MeshFactory const &) {}

    static MeshFactory* instance;
    std::map<Mesh *, AttribStructBase *> meshToVertWrapperMap;

    uint32_t idCounter = 0;
    uint32_t GenerateMeshId() { return idCounter++; }

    int * CreateVertexBuffer(uint32_t count);
    int CreateIndexBuffer();

    std::bitset<(uint32_t)ATTRIBUTES::NUM_ATTRIBUTES> pcMask; 
    std::bitset<(uint32_t)ATTRIBUTES::NUM_ATTRIBUTES> pcnMask;

public:
    void Init(ApiInterface * obj);
    void DeInit();
    void Update();
    static MeshFactory* GetInstance();
    ~MeshFactory();

    Mesh * CreateMesh(const BitArray & attribMaskReq, MESH_TYPE * meshType, bool isIndexed, bool individualBuffersRequired);
    Mesh * CreateMesh(const MeshInfo * meshInfo, MESH_TYPE * meshType);
    void DestroyMesh(const uint32_t & meshId);
};
