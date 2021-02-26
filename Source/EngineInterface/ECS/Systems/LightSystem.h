#pragma once
#include "System.h"
#include <glm\glm.hpp>
#include "ShaderResourceDescription.h"
#include "DrawGraphNode.h"
#include "MeshAdditionEvent.h"
#include "ComponentAdditionEvent.h"

template <typename T>
class GraphNode;

template <typename T>
class ComponentHandle;

class Light;

class LightDrawNode : public DrawGraphNode
{
public:
    std::vector<uint32_t> descriptorIds;
    virtual void Entry() override;
    //virtual void Exit() override;
    LightDrawNode()
    {
        drawNodeType = DrawNodeTypes::LIGHT;
    }
};


class LightSystem : public System
{
private:
    std::vector<Light *> lightlist;
    std::vector<ShaderBindingDescription *> resDescriptionList;
    GlobalResourceAllocationConfig allocConfig;
    GlobalResourceSharingConfig resourceSharingConfig;

    uint32_t idCounter = 0;
    uint32_t GeneratedLightId();

    SetWrapper * lightSetWrapper;

    std::vector<GraphNode<DrawGraphNode> *> lightGraphNodeList;
    std::map<DrawGraphNode *, ShaderBindingDescription *> nodeToDescriptionMap;
    std::map<Light *, ShaderBindingDescription *> lightToDescriptionMap;

    size_t memoryAlignedUniformSize;

public:
    virtual void Init() override;
    virtual void DeInit() override;
    virtual void Update(float dt) override;

    void HandleLightAddition(LightAdditionEvent * lightAdditionEvent);
    void HandleMeshAddition(MeshAdditionEvent *  meshAdditionEvent);

    LightSystem();
    virtual ~LightSystem();
};
