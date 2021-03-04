#include "LightSystem.h"
#include "EventBus.h"
#include "ComponentAdditionEvent.h"
#include "Light.h"
#include "ResourceAllocationHelper.h"
#include "UniformFactory.h"
#include "Transform.h"
#include "MaterialFactory.h"
#include "DrawGraphManager.h"
#include "World.h"

uint32_t LightSystem::GeneratedLightId()
{
    return idCounter++;
}

void LightSystem::Init()
{
    EventBus::GetInstance()->Subscribe<LightSystem, LightAdditionEvent>(this, &LightSystem::HandleLightAddition);
    EventBus::GetInstance()->Subscribe<LightSystem, MeshAdditionEvent>(this, &LightSystem::HandleMeshAddition);

    allocConfig.numDescriptors = Settings::maxFramesInFlight;
    allocConfig.numMemories = 1;
    allocConfig.numResources = 1;

    resourceSharingConfig.maxUniformPerResource = 1;
    resourceSharingConfig.allocatedUniformCount = 0;

    size_t uniformSize = sizeof(LightUniform);
    memoryAlignedUniformSize = UniformFactory::GetInstance()->GetMemoryAlignedDataSizeForBuffer(uniformSize);
}

void LightSystem::DeInit()
{
    for (uint32_t i = 0; i < resDescriptionList.size(); i++)
    {
        delete resDescriptionList[i];
    }
    resDescriptionList.clear();
}

void LightSystem::Update(float dt)
{
    return;
    for (auto & entity : registeredEntities)
    {
        ComponentHandle<Light> * lightHandle;
        worldObj->Unpack(entity, &lightHandle);

        Light * light = lightHandle->GetComponent();
        LightUniform obj = {};
        obj.ambient = Vec3ToVec4_0(light->GetAmbient());
        obj.diffuse = Vec3ToVec4_0(light->GetDiffuse());
        obj.specular = Vec3ToVec4_0(light->GetSpecular());
        obj.lightPos = Vec3ToVec4_0(light->GetTransform()->GetGlobalPosition());

        ShaderBindingDescription * desc = lightToDescriptionMap[lightHandle->GetComponent()];

        //upload data to buffers
        {
            UniformFactory::GetInstance()->UploadDataToBuffers(desc->resourceId, sizeof(LightUniform), memoryAlignedUniformSize, &obj, desc->offsetsForEachDescriptor[Settings::currentFrameInFlight], false);
        }

        // TODO : write the uniform data of Camera to gpu memory via void*
    }
}

void LightSystem::HandleLightAddition(LightAdditionEvent * lightAdditionEvent)
{
    // recieved the light addition to the scene 
    Light * light = lightAdditionEvent->light;

    lightlist.push_back(light);
    light->componentId = GeneratedLightId();

    ShaderBindingDescription * desc = new ShaderBindingDescription;
    desc->set = (uint32_t)ResourceSets::LIGHT;
    desc->binding = 0;
    desc->numElements = 4;
    desc->resourceName = "Lights";
    desc->resourceType = DescriptorType::UNIFORM_BUFFER;
    desc->parentType = light->componentType;
    desc->dataSizePerDescriptorAligned = memoryAlignedUniformSize;
    desc->dataSizePerDescriptor = sizeof(LightUniform);
    desc->uniformId = light->componentId; // as one buffer is getting used to for all the descriptors
    desc->offsetsForEachDescriptor = AllocationUtility::CalculateOffsetsForDescInUniform(memoryAlignedUniformSize, allocConfig, resourceSharingConfig);
    desc->allocationConfig = allocConfig;

    // Check if it can be fit into an existing buffer
    if (AllocationUtility::IsNewAllocationRequired(resourceSharingConfig))
    {
        size_t totalSize = AllocationUtility::GetDataSizeMeantForSharing(memoryAlignedUniformSize, allocConfig, resourceSharingConfig);
        // True : Allocate new buffer
        lightSetWrapper = UniformFactory::GetInstance()->AllocateResource(desc, &totalSize, 1, AllocationMethod::LAZY);
    }
    else
    {
        // False : Assign the buffer id to this shaderResourceDescription
        desc->resourceId = resDescriptionList[resDescriptionList.size() - 1]->resourceId;
        desc->resourceMemoryId = resDescriptionList[resDescriptionList.size() - 1]->resourceMemoryId;
    }

    resDescriptionList.push_back(desc);
    resourceSharingConfig.allocatedUniformCount += 1;

    LightUniform obj = {};
    obj.ambient = Vec3ToVec4_0(light->GetAmbient());
    obj.diffuse = Vec3ToVec4_0(light->GetDiffuse());
    obj.specular = Vec3ToVec4_0(light->GetSpecular());
    obj.lightPos = Vec3ToVec4_0(light->GetTransform()->GetGlobalPosition());
    
    //upload data to buffers
    for (uint32_t i = 0; i < allocConfig.numDescriptors; i++)
    {
        UniformFactory::GetInstance()->UploadDataToBuffers(desc->resourceId, sizeof(LightUniform),
            memoryAlignedUniformSize, &obj, desc->offsetsForEachDescriptor[i], false);
    }

    UniformFactory::GetInstance()->AllocateDescriptors(lightSetWrapper, desc, 1, allocConfig.numDescriptors);

    lightToDescriptionMap.insert(std::pair<Light *, ShaderBindingDescription *>(
    { light, desc }));

    // draw graph node creation
    // top level node as its Set 0
    DrawGraphNode * lightNode = new LightDrawNode();
    lightNode->meshList = MaterialFactory::GetInstance()->GetMeshList(lightSetWrapper, 1);
    lightNode->setLevel = lightSetWrapper->setValue;
    lightNode->setWrapperList.push_back(lightSetWrapper);

    ((LightDrawNode*)lightNode)->descriptorIds = desc->descriptorIds;

    GraphNode<DrawGraphNode> * graphNode = new GraphNode<DrawGraphNode>(lightNode);
    lightGraphNodeList.push_back(graphNode);

    DrawGraphManager::GetInstance()->AddNode(graphNode);
}

void LightSystem::HandleMeshAddition(MeshAdditionEvent * meshAdditionEvent)
{
    uint32_t setCount = (uint32_t)meshAdditionEvent->setWrapperList.size();
    for (uint32_t k = 0; k < lightGraphNodeList.size(); k++)
        for (uint32_t i = 0; i < setCount; i++)
        {
            SetWrapper * wrapper = meshAdditionEvent->setWrapperList[i];
            if (wrapper->id == lightSetWrapper->id)
            {
                std::vector<uint32_t> meshList = lightGraphNodeList[k]->node->meshList;
                std::vector<uint32_t>::iterator it;
                it = std::find_if(meshList.begin(), meshList.end(), [=](uint32_t id) {
                    return id == meshAdditionEvent->meshId; });

                if (it == meshList.end())
                {
                    lightGraphNodeList[k]->node->meshList.push_back(meshAdditionEvent->meshId);
                }
            }
        }
}

LightSystem::LightSystem()
{
    signature.AddComponent<Light>();
}

LightSystem::~LightSystem()
{
}

