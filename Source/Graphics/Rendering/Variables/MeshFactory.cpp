
#include "MeshFactory.h"
#include <Mesh.h>
#include <AttributeHelper.h>
#include <stdint.h>
#include <RenderingWrapper.h>
#include <algorithm>
#include <Assertion.h>

#if (RENDERING_API == VULKAN)
#include <VulkanInterface.h>
#elif (RENDERING_API == DX)
#include <DxInterface.h>
#endif

MeshFactory* MeshFactory::instance = nullptr;

int * MeshFactory::CreateVertexBuffer(uint32_t count)
{
    return nullptr;
}

int MeshFactory::CreateIndexBuffer()
{
    return 0;
}

void MeshFactory::DeInit()
{
    std::map<Mesh *, WrapperBase *>::iterator it;

    for (it = meshToVertWrapperMap.begin(); it != meshToVertWrapperMap.end(); it++)
    {
        delete it->second;
    }
}

void MeshFactory::Update()
{

}

MeshFactory * MeshFactory::GetInstance()
{
    if (instance == nullptr)
    {
        instance = new MeshFactory();
    }
    return instance;
}

MeshFactory::~MeshFactory()
{
}

Mesh * MeshFactory::CreateMesh(const BitArray & attribMaskReq, MESH_TYPE * meshType, bool isIndexed, bool individualBuffersRequired)
{
    Mesh * mesh = new Mesh();
    
    if (attribMaskReq[(unsigned int)ATTRIBUTES::POSITION] && attribMaskReq[(unsigned int)ATTRIBUTES::COLOR])
    {
        WrapperBase * wrapper = new WrapperPC();
        if (isIndexed)
            ((WrapperPC*)wrapper)->FillData<CubeIndexed>(mesh);

        mesh->vertexAttributeCount = 2;
        mesh->meshId = GetId();
        mesh->vertexCount = wrapper->vertexCount;
        if (individualBuffersRequired == false)
        {
            BufferType bufferType = BufferType::VERTEX_BUFFER_BIT;
            MemoryType memType = MemoryType::HOST_VISIBLE_BIT;
            BufferInfo info = {};
            info.bufType = &bufferType;
            info.memType = &memType;
            info.data = wrapper->vertexData;
            info.dataSize = wrapper->vertexDataSize;
            info.pGpuMem = new PC;

            mesh->vertexBufferCount = 1;
            mesh->vertexBuffersIds = apiInterface->CreateBuffer(&info, 1);
            mesh->pGpuMemVB = new void*[1];
            mesh->pGpuMemVB[0] = info.pGpuMem;
        }

        BufferType bufferType = BufferType::INDEX_BUFFER_BIT;
        MemoryType memType = MemoryType::HOST_VISIBLE_BIT;
        BufferInfo info = {};
        info.bufType = &bufferType;
        info.memType = &memType;
        info.data = wrapper->indexData;
        info.dataSize = wrapper->indexDataSize;
        info.pGpuMem = new uint32_t;
        mesh->vertexCount = wrapper->indexCount;
        mesh->indexBufferId = *apiInterface->CreateBuffer(&info, 1);
        mesh->pGpuMemIB = info.pGpuMem;

        meshToVertWrapperMap.insert({mesh, wrapper});
    }

    return mesh;
}

void MeshFactory::DestroyMesh(const uint32_t & meshId)
{
    std::map<Mesh *, WrapperBase *>::iterator it;
    std::pair<Mesh *, WrapperBase *> obj;
    it = std::find_if(meshToVertWrapperMap.begin(), meshToVertWrapperMap.end(), 
        [&](std::pair<Mesh *, WrapperBase *> obj) { return obj.first->meshId == meshId; });

    ASSERT_MSG(it != meshToVertWrapperMap.end(), "Image id not found");

    apiInterface->DestroyBuffer((it)->first->vertexBuffersIds, (it)->first->vertexBufferCount);
    apiInterface->DestroyBuffer(&(it)->first->indexBufferId, 1);
}
