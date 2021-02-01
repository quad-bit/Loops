#include "MeshRendererSystem.h"
#include "Mesh.h"
#include "MeshRenderer.h"
#include "Material.h"
#include "Transform.h"
#include "World.h"
#include "EventBus.h"
#include "ComponentAdditionEvent.h"
#include "ResourceAllocationHelper.h"
#include "UniformFactory.h"
#include "DrawGraphManager.h"
#include "Graph.h"

uint32_t MeshRendererSystem::GenerateId()
{
    return idCounter++;
}

void MeshRendererSystem::Init()
{
    EventBus::GetInstance()->Subscribe<MeshRendererSystem, MeshRendererAdditionEvent>(this, &MeshRendererSystem::HandleMeshRendererAddition);

    allocConfig.numDescriptors = Settings::maxFramesInFlight;
    allocConfig.numMemories = 1;
    allocConfig.numResources = 1;

    resourceSharingConfig.maxUniformPerResource = 2;
    resourceSharingConfig.allocatedUniformCount = 0;
}

void MeshRendererSystem::DeInit()
{
    for each(auto obj in meshNodeList)
    {
        delete obj->node;
        delete obj;
    }

    meshNodeList.clear();
    
    for each(auto obj in transformNodeList)
    {
        delete obj->node;
        delete obj;
    }

    transformNodeList.clear();
}

void MeshRendererSystem::Update(float dt)
{
    for (auto & entity : registeredEntities)
    {
        ComponentHandle<MeshRenderer> * renderer;
        ComponentHandle<Transform> * transform;
        worldObj->Unpack(entity, &renderer, &transform);
    }
}

MeshRendererSystem::MeshRendererSystem()
{
    signature.AddComponent<MeshRenderer>();
    signature.AddComponent<Mesh>();
    signature.AddComponent<Material>();
    signature.AddComponent<Transform>();
}

MeshRendererSystem::~MeshRendererSystem()
{
}

void MeshRendererSystem::HandleMeshRendererAddition(MeshRendererAdditionEvent * inputEvent)
{
    // Create descriptor for transform set
    inputEvent->renderer->componentId = GenerateId();

    size_t uniformSize = sizeof(TransformUniform);

    ShaderBindingDescription * desc = new ShaderBindingDescription;
    desc->set = 4;
    desc->binding = 0;
    desc->numElements = 1;
    desc->resourceName = "Transform";
    desc->resourceType = DescriptorType::UNIFORM_BUFFER;
    desc->resParentId = inputEvent->renderer->componentId;
    desc->parentType = inputEvent->renderer->type;
    desc->dataSizePerDescriptor = uniformSize;
    desc->uniformId = inputEvent->renderer->componentId; 
    desc->offsetsForEachDescriptor = AllocationUtility::CalculateOffsetsForDescInUniform(uniformSize, allocConfig, resourceSharingConfig);
    desc->allocationConfig = allocConfig;

    // Check if it can be fit into an existing buffer
    if (AllocationUtility::IsNewAllocationRequired(resourceSharingConfig))
    {
        size_t totalSize = AllocationUtility::GetDataSizeMeantForSharing(uniformSize, allocConfig, resourceSharingConfig);
        // True : Allocate new buffer
        transformSetWrapper = UniformFactory::GetInstance()->AllocateResource(desc, &totalSize, 1, AllocationMethod::LAZY);
    }
    else
    {
        // False : Assign the buffer id to this shaderResourceDescription
        desc->resourceId = resDescriptionList[resDescriptionList.size() - 1]->resourceId;
        desc->resourceMemoryId = resDescriptionList[resDescriptionList.size() - 1]->resourceMemoryId;
    }

    resDescriptionList.push_back(desc);
    resourceSharingConfig.allocatedUniformCount += 1;

    UniformFactory::GetInstance()->AllocateDescriptors(transformSetWrapper, desc, 1, allocConfig.numDescriptors);

    // Create transform descriptor node.
    uint32_t meshId = inputEvent->renderer->geometry->componentId;
 
    DrawGraphNode * meshNode = new MeshNode;
    meshNode->setWrapper = nullptr;
    meshNode->meshList.push_back(meshId);

    DrawGraphNode * trfnode = new TransformNode;
    trfnode->setWrapper = transformSetWrapper;
    trfnode->meshList.push_back(meshId);
    trfnode->setLevel = transformSetWrapper->setValue;

    GraphNode<DrawGraphNode> * transformGraphNode = new GraphNode<DrawGraphNode>(trfnode);
    GraphNode<DrawGraphNode> * meshGraphNode = new GraphNode<DrawGraphNode>(meshNode);
    DrawGraphManager::GetInstance()->AddNode(transformGraphNode);
    DrawGraphManager::GetInstance()->AddNode(meshGraphNode);
    
    meshNodeList.push_back(meshGraphNode);
    transformNodeList.push_back(transformGraphNode);
    
    DrawGraphManager::GetInstance()->CreateGraphEdges(meshGraphNode, transformGraphNode);
}

void TransformNode::Execute()
{

}

void MeshNode::Execute()
{
}
