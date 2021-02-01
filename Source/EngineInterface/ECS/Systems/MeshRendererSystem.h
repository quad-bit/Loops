
#pragma once
#include "System.h"
#include "ShaderResourceDescription.h"
#include "DrawGraphNode.h"

template <typename T>
class GraphNode;

class MeshRenderer;
class MeshRendererAdditionEvent;

class TransformNode : public DrawGraphNode
{
public:
    virtual void Execute() override;
    TransformNode()
    {
        numMeshes = 1;
        drawNodeType = DrawNodeTypes::TRANSFORM;
    }
};

class MeshNode : public DrawGraphNode
{
public:
    virtual void Execute() override;
    MeshNode()
    {
        numMeshes = 1; //  for now vertex buffer and index buffer are not getting shared
        // between multiple objects
        drawNodeType = DrawNodeTypes::MESH;
    }
};


class MeshRendererSystem : public System
{
private:

    uint32_t idCounter = 0;
    uint32_t GenerateId();

    GlobalResourceAllocationConfig allocConfig;
    GlobalResourceSharingConfig resourceSharingConfig;

    SetWrapper * transformSetWrapper;
    std::vector<ShaderBindingDescription *> resDescriptionList;
    uint32_t numDescriptorsPerBinding;
    
    std::map<uint32_t, DrawGraphNode*> meshIdToGraphNode;

    std::vector<GraphNode<DrawGraphNode> * >  meshNodeList;
    std::vector<GraphNode<DrawGraphNode> * >  transformNodeList;

public:
    virtual void Init() override;
    virtual void DeInit() override;
    virtual void Update(float dt) override;

    MeshRendererSystem();
    virtual ~MeshRendererSystem();

    void HandleMeshRendererAddition(MeshRendererAdditionEvent * inputEvent);
};
