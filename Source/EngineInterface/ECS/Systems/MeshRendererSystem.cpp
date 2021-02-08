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
#include <plog/Log.h> 
#include "DrawCommandBuffer.h"
#include "DrawGraphNode.h"

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

    for each(auto obj in drawingNodeList)
    {
        delete obj->node;
        delete obj;
    }

    drawingNodeList.clear();
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
        // True : Allocate new buffer
        size_t totalSize = AllocationUtility::GetDataSizeMeantForSharing(uniformSize, allocConfig, resourceSharingConfig);
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
    
    TransformUniform obj = {};
    obj.modelMat = inputEvent->renderer->transform->globalModelMatrix;

    //upload data to buffers
    for (uint32_t i = 0; i < allocConfig.numDescriptors; i++)
    {
        UniformFactory::GetInstance()->UploadDataToBuffers(desc->resourceId, desc->dataSizePerDescriptor, &obj, desc->offsetsForEachDescriptor[i], false);
    }
    
    // allocate descriptors
    UniformFactory::GetInstance()->AllocateDescriptors(transformSetWrapper, desc, 1, allocConfig.numDescriptors);
    
    uint32_t meshId = inputEvent->renderer->geometry->componentId;
    
    DrawGraphNode * meshNode = new MeshNode;
    {
        meshNode->meshList.push_back(meshId);
        uint32_t numVertBuffers = inputEvent->renderer->geometry->vertexBufferCount;
        for (uint32_t i = 0; i < numVertBuffers; i++)
            ((MeshNode*)meshNode)->bufferIds.push_back(inputEvent->renderer->geometry->vertexBuffersIds[i]);
        ((MeshNode*)meshNode)->pOffsets.push_back(0);
        ((MeshNode*)meshNode)->indexBufferId = inputEvent->renderer->geometry->indexBufferId;
    }

    DrawGraphNode * trfnode = new TransformNode;
    {
        trfnode->setWrapperList.push_back(transformSetWrapper);
        trfnode->meshList.push_back(meshId);
        trfnode->setLevel = transformSetWrapper->setValue;
        ((TransformNode*)trfnode)->descriptorSetIds = desc->descriptorIds;
    }

    DrawGraphNode * drawingnode = new IndexedDrawNode;
    {
        drawingnode->meshList.push_back(meshId);

        ((IndexedDrawNode*)drawingnode)->info.firstIndex = 0;
        ((IndexedDrawNode*)drawingnode)->info.firstInstance = 0;
        ((IndexedDrawNode*)drawingnode)->info.indexCount = inputEvent->renderer->geometry->indexCount;
        ((IndexedDrawNode*)drawingnode)->info.instanceCount = 1;
        ((IndexedDrawNode*)drawingnode)->info.vertexOffset = 0;
    }

    GraphNode<DrawGraphNode> * transformGraphNode = new GraphNode<DrawGraphNode>(trfnode);
    GraphNode<DrawGraphNode> * meshGraphNode = new GraphNode<DrawGraphNode>(meshNode);
    GraphNode<DrawGraphNode> * drawingGraphNode = new GraphNode<DrawGraphNode>(drawingnode);
    DrawGraphManager::GetInstance()->AddNode(transformGraphNode);
    DrawGraphManager::GetInstance()->AddNode(meshGraphNode);
    DrawGraphManager::GetInstance()->AddNode(drawingGraphNode);
    
    meshNodeList.push_back(meshGraphNode);
    transformNodeList.push_back(transformGraphNode);
    drawingNodeList.push_back(drawingGraphNode);
    
    DrawGraphManager::GetInstance()->CreateGraphEdges(meshGraphNode, transformGraphNode);
    DrawGraphManager::GetInstance()->CreateGraphEdges(transformGraphNode, drawingGraphNode);
}

void TransformNode::Entry()
{
    PipelineType bindPoint = PipelineType::GRAPHICS;

    // Binding the descriptor set for transform
    DescriptorSetBindingInfo info = {};
    info.descriptorSetId = (this->descriptorSetIds[Settings::currentFrameInFlight]);
    info.firstSet = DrawGraphUtil::setOffset;
    info.dynamicOffsetCount = 0;
    info.pDynamicOffsets = nullptr;
    info.pipelineBindPoint = &bindPoint;
    info.pipelineLayoutId = DrawGraphUtil::pipelineLayoutId;
    
    DrawGraphUtil::descriptorIdList.push_back(info.descriptorSetId);

    info.descriptorSetIds = (DrawGraphUtil::descriptorIdList);
    dcb->BindDescriptorSets(&info);
}

void MeshNode::Entry()
{
    VertexBufferBindingInfo info = {};
    info.bindingCount = 1;
    info.firstBinding = 0;
    info.bufferIds = this->bufferIds;
    info.pOffsets = this->pOffsets;
    dcb->BindVertexBuffers(&info);

    IndexBufferBindingInfo indInfo = {};
    indInfo.bufferId = this->indexBufferId;
    indInfo.indexType = IndexType::INDEX_TYPE_UINT32;
    indInfo.offset = 0;
    dcb->BindIndexBuffer(&indInfo);
}

void MeshNode::Exit()
{

}

void IndexedDrawNode::Entry()
{
    dcb->DrawIndex(&this->info);
}

void IndexedDrawNode::Exit()
{

}
