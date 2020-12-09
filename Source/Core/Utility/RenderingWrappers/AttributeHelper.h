#pragma once

#include <glm\glm.hpp>
#include <map>
#include <vector>
#include <Cube.h>
#include <stdint.h>


class Mesh;

enum class ATTRIBUTES
{
    POSITION = 0,
    COLOR = 1,
    NORMAL = 2,
    TANGENT = 3,
    UV = 4,
    UV1 = 5,
    UV2 = 6,
    UV3 = 7,
    UV4 = 8
};

struct VertexMetaData
{
    uint32_t vertexDataStride, indexDataStride;
    std::map<ATTRIBUTES, uint32_t> offets;
    std::map<ATTRIBUTES, uint32_t> locations;
};

struct PC
{
    glm::vec3 position;
    glm::uvec4 color;
};

struct WrapperBase
{
    void * vertexData;
    uint32_t vertexDataSize;
    VertexMetaData metaData;
    uint32_t vertexCount;

    void * indexData;
    uint32_t indexDataSize;
    uint32_t indexCount;

    WrapperBase() {};
};


struct WrapperPC : public WrapperBase
{
    std::vector<PC> posColList;
    std::vector<uint32_t> indicies;
    WrapperPC()
    {
        
        metaData.vertexDataStride = sizeof(PC);
        metaData.offets.insert(std::pair<ATTRIBUTES, uint32_t>(ATTRIBUTES::POSITION, (uint32_t)offsetof(PC, PC::position)));
        metaData.offets.insert(std::pair<ATTRIBUTES, uint32_t>(ATTRIBUTES::COLOR, (uint32_t)offsetof(PC, PC::color)));
        metaData.locations.insert(std::pair<ATTRIBUTES, uint32_t>(ATTRIBUTES::POSITION, 0));
        metaData.locations.insert(std::pair<ATTRIBUTES, uint32_t>(ATTRIBUTES::COLOR, 1));

        metaData.indexDataStride = sizeof(uint32_t);
    }

    ~WrapperPC()
    {
        metaData.locations.clear();
        metaData.offets.clear();
    }

    template<typename T>
    void FillData(Mesh * mesh);
    
};

#include <Mesh.h>

template<typename T>
inline void WrapperPC::FillData(Mesh * mesh)
{
    T obj;

    size_t numVertices = obj.positions.size();
    posColList.resize(numVertices);

    mesh->positions.resize(numVertices);
    mesh->colors.resize(numVertices);
    
    for (uint32_t i = 0; i < numVertices; i++)
    {
        posColList[i].position = obj.positions[i];
        posColList[i].color = obj.color;
        mesh->positions[i] = &posColList[i].position;
        mesh->colors[i] = &posColList[i].color;
    }

    vertexData = posColList.data();
    vertexDataSize = (uint32_t)posColList.size() * sizeof(PC);
    vertexCount = numVertices;

    size_t numIndicies = obj.indices.size();

    indicies.resize(numIndicies);
    mesh->indicies.resize(numIndicies);

    for (uint32_t i = 0; i < numIndicies; i++)
    {
        indicies[i] = obj.indices[i];
        mesh->indicies[i] = &indicies[i];
    }

    indexData = indicies.data();
    indexDataSize = (uint32_t)indicies.size() * sizeof( uint32_t );
    indexCount = numIndicies;
}
