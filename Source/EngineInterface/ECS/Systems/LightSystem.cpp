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
#include "Mesh.h"
#include "Camera.h"
#include "CameraSystem.h"
#include "RendererSettings.h"

uint32_t LightSystem::GeneratedLightId()
{
    return idCounter++;
}

void LightSystem::CreateLightUniformDescription(ShaderBindingDescription * desc, Light * light)
{
    desc->set = (uint32_t)ResourceSets::LIGHT;
    desc->binding = 0;
    desc->numElements = 4;
    desc->resourceName = "Lights";
    desc->resourceType = DescriptorType::UNIFORM_BUFFER;
    desc->parentType = light->componentType;
    desc->uniformId = light->componentId; 
    desc->bufferBindingInfo.info.dataSizePerDescriptorAligned = memoryAlignedUniformSize;
    desc->bufferBindingInfo.info.dataSizePerDescriptor = sizeof(LightUniform);
    desc->bufferBindingInfo.info.offsetsForEachDescriptor = AllocationUtility::CalculateOffsetsForDescInUniform(memoryAlignedUniformSize, lightUniformAllocConfig, lightBufferSharingConfig);
    desc->bufferBindingInfo.info.allocationConfig = lightUniformAllocConfig;
    desc->bufferBindingInfo.bufferIdList.resize(lightUniformAllocConfig.numResources);
    desc->bufferBindingInfo.bufferMemoryId.resize(lightUniformAllocConfig.numMemories);
}

void LightSystem::CreateLightUniformBuffer(ShaderBindingDescription * desc, Light * light)
{
    // Check if it can be fit into an existing buffer
    if (AllocationUtility::IsNewAllocationRequired(lightBufferSharingConfig))
    {
        size_t totalSize = AllocationUtility::GetDataSizeMeantForSharing(memoryAlignedUniformSize, lightUniformAllocConfig, lightBufferSharingConfig);
        // True : Allocate new buffer, this should be Allocate resource for binding instead of complete set
        //lightSetWrapper = UniformFactory::GetInstance()->AllocateSetResource(desc, &totalSize, 1, AllocationMethod::LAZY);
        UniformFactory::GetInstance()->AllocateBindingResources(lightSetWrapper , desc, totalSize, 0, AllocationMethod::LAZY);
    }
    else
    {
        // False : Assign the buffer id to this shaderResourceDescription
        desc->bufferBindingInfo.bufferIdList[0] = resDescriptionList[resDescriptionList.size() - 1]->bufferBindingInfo.bufferIdList[0];
        desc->bufferBindingInfo.bufferMemoryId[0] = resDescriptionList[resDescriptionList.size() - 1]->bufferBindingInfo.bufferMemoryId[0];
    }

    resDescriptionList.push_back(desc);
    lightBufferSharingConfig.allocatedUniformCount += 1;

    LightUniform obj = {};
    obj.ambient = Vec3ToVec4_0(light->GetAmbient());
    obj.diffuse = Vec3ToVec4_0(light->GetDiffuse());
    obj.specular = Vec3ToVec4_0(light->GetSpecular());
    obj.lightPos = Vec3ToVec4_0(light->GetTransform()->GetGlobalPosition());

    //upload data to buffers
    for (uint32_t i = 0; i < lightUniformAllocConfig.numDescriptorSets; i++)
    {
        UniformFactory::GetInstance()->UploadDataToBuffers(desc->bufferBindingInfo.bufferIdList[0], sizeof(LightUniform),
            memoryAlignedUniformSize, &obj, desc->bufferBindingInfo.info.offsetsForEachDescriptor[i], false);
    }
}

void LightSystem::CreateLightCamera(Transform * transform)
{
    // create a camera 
    Camera * lightCam = new Camera(transform);
    //lightCam->SetProjectionType(CameraType::ORTHOGONAL);
    cameralist.push_back(lightCam);

    // create the camera draw node, node has been added to the graph in HandleCameraAddition
    GraphNode<DrawGraphNode> * cameraNode = ((CameraSystem*)cameraSystem)->HandleCameraAddition(lightCam, RenderPassTag::DepthPass);
    cameraNode->node->tag = RenderPassTag::DepthPass;
}

void LightSystem::CreateShadowMap(ShaderBindingDescription * desc)
{
    SamplerCreateInfo info = {};
    info.minFilter = Filter::FILTER_LINEAR;
    info.magFilter = Filter::FILTER_LINEAR;
    info.addressModeU = SamplerAddressMode::SAMPLER_ADDRESS_MODE_REPEAT;
    info.addressModeV = SamplerAddressMode::SAMPLER_ADDRESS_MODE_REPEAT;
    info.addressModeW = SamplerAddressMode::SAMPLER_ADDRESS_MODE_REPEAT;
    info.anisotropyEnable = false;
    info.maxAnisotropy = 1.0f;
    info.borderColor = BorderColor::BORDER_COLOR_INT_OPAQUE_BLACK;
    info.unnormalizedCoordinates = false;
    info.compareEnable = false;
    info.compareOp = CompareOp::COMPARE_OP_ALWAYS;
    info.mipmapMode = SamplerMipmapMode::SAMPLER_MIPMAP_MODE_NEAREST;
    info.minLod = 0.0f;
    info.maxLod = 0.0f;
    info.mipLodBias = 0.0f;

    uint32_t samplerId = UniformFactory::GetInstance()->CreateSampler(info);

    std::vector<uint32_t> temp = RendererSettings::depthPrepassImageId;

    desc->set = (uint32_t)ResourceSets::LIGHT;
    desc->binding = 1;
    desc->numElements = 0;
    desc->resourceName = "combined_shadowSampler";
    desc->resourceType = DescriptorType::COMBINED_IMAGE_SAMPLER;
    desc->imageBindingInfo.imageId = RendererSettings::depthPrepassImageId;
    desc->samplerBindingInfo.samplerId.push_back(samplerId);
    //desc->parentType = light->componentType;
    //desc->dataSizePerDescriptorAligned = memoryAlignedUniformSize;
    //desc->dataSizePerDescriptor = sizeof(LightUniform);
    //desc->uniformId = light->componentId; // as one buffer is getting used to for all the descriptors
    //desc->offsetsForEachDescriptor = AllocationUtility::CalculateOffsetsForDescInUniform(memoryAlignedUniformSize, lightUniformAllocConfig, lightBufferSharingConfig);
    //desc->allocationConfig = lightUniformAllocConfig;
}

void LightSystem::Init()
{
    EventBus::GetInstance()->Subscribe<LightSystem, LightAdditionEvent>(this, &LightSystem::HandleLightAddition);
    EventBus::GetInstance()->Subscribe<LightSystem, MeshToMatAdditionEvent>(this, &LightSystem::HandleMeshAddition);
    EventBus::GetInstance()->Subscribe<LightSystem, MeshRendererAdditionEvent>(this, &LightSystem::HandleRendererAddition);

    lightUniformAllocConfig.numDescriptorSets = Settings::swapBufferCount;
    lightUniformAllocConfig.numMemories = 1;
    lightUniformAllocConfig.numResources = 1;

    shadowMapUniformAllocConfig.numDescriptorSets = Settings::swapBufferCount;
    shadowMapUniformAllocConfig.numMemories = 1;
    shadowMapUniformAllocConfig.numResources = 1;

    lightBufferSharingConfig.maxUniformPerResource = 1;
    lightBufferSharingConfig.allocatedUniformCount = 0;

    size_t uniformSize = sizeof(LightUniform);
    memoryAlignedUniformSize = UniformFactory::GetInstance()->GetMemoryAlignedDataSizeForBuffer(uniformSize);
}

void LightSystem::DeInit()
{
    for (uint32_t i = 0; i < resDescriptionList.size(); i++)
    {
        delete [] resDescriptionList[i];
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
            UniformFactory::GetInstance()->UploadDataToBuffers(desc->bufferBindingInfo.bufferIdList[0], 
                sizeof(LightUniform), memoryAlignedUniformSize, &obj, 
                desc->bufferBindingInfo.info.offsetsForEachDescriptor[Settings::currentFrameInFlight], false);
        }

        // TODO : write the uniform data of Camera to gpu memory via void*
    }
}

/*
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
    desc->uniformId = light->componentId; // as one buffer is getting used to for all the descriptors
    desc->bufferBindingInfo.info.dataSizePerDescriptorAligned = memoryAlignedUniformSize;
    desc->bufferBindingInfo.info.dataSizePerDescriptor = sizeof(LightUniform);
    desc->bufferBindingInfo.info.offsetsForEachDescriptor = AllocationUtility::CalculateOffsetsForDescInUniform(memoryAlignedUniformSize, lightUniformAllocConfig, lightBufferSharingConfig);
    desc->bufferBindingInfo.info.allocationConfig = lightUniformAllocConfig;
    desc->bufferBindingInfo.bufferIdList.resize(lightUniformAllocConfig.numResources);
    desc->bufferBindingInfo.bufferMemoryId.resize(lightUniformAllocConfig.numMemories);

    // Check if it can be fit into an existing buffer
    if (AllocationUtility::IsNewAllocationRequired(lightBufferSharingConfig))
    {
        size_t totalSize = AllocationUtility::GetDataSizeMeantForSharing(memoryAlignedUniformSize, lightUniformAllocConfig, lightBufferSharingConfig);
        // True : Allocate new buffer
        lightSetWrapper = UniformFactory::GetInstance()->AllocateSetResource(desc, &totalSize, 1, AllocationMethod::LAZY);
    }
    else
    {
        // False : Assign the buffer id to this shaderResourceDescription
        desc->bufferBindingInfo.bufferIdList[0] = resDescriptionList[resDescriptionList.size() - 1]->bufferBindingInfo.bufferIdList[0];
        desc->bufferBindingInfo.bufferMemoryId[0] = resDescriptionList[resDescriptionList.size() - 1]->bufferBindingInfo.bufferMemoryId[0];
    }

    resDescriptionList.push_back(desc);
    lightBufferSharingConfig.allocatedUniformCount += 1;

    LightUniform obj = {};
    obj.ambient = Vec3ToVec4_0(light->GetAmbient());
    obj.diffuse = Vec3ToVec4_0(light->GetDiffuse());
    obj.specular = Vec3ToVec4_0(light->GetSpecular());
    obj.lightPos = Vec3ToVec4_0(light->GetTransform()->GetGlobalPosition());
    
    //upload data to buffers
    for (uint32_t i = 0; i < lightUniformAllocConfig.numDescriptorSets; i++)
    {
        UniformFactory::GetInstance()->UploadDataToBuffers(desc->bufferBindingInfo.bufferIdList[0], sizeof(LightUniform),
            memoryAlignedUniformSize, &obj, desc->bufferBindingInfo.bufferIdList[0], false);
    }

    UniformFactory::GetInstance()->AllocateDescriptors(lightSetWrapper, desc, 1, lightUniformAllocConfig.numDescriptorSets);

    lightToDescriptionMap.insert(std::pair<Light *, ShaderBindingDescription *>(
    { light, desc }));

    // draw graph node creation
    // top level node as its Set 0
    DrawGraphNode * lightNode = new LightDrawNode();
    lightNode->meshList = MaterialFactory::GetInstance()->GetMeshList(lightSetWrapper, 1);
    lightNode->setLevel = lightSetWrapper->setValue;
    lightNode->setWrapperList.push_back(lightSetWrapper);
    lightNode->tag = RenderPassTag::ColorPass;

    ((LightDrawNode*)lightNode)->descriptorIds = desc->descriptorSetIds;

    GraphNode<DrawGraphNode> * graphNode = new GraphNode<DrawGraphNode>(lightNode);
    lightGraphNodeList.push_back(graphNode);

    DrawGraphManager::GetInstance()->AddNode(graphNode);

    // create a camera 
    Camera * lightCam = new Camera(light->GetTransform());
    //lightCam->SetProjectionType(CameraType::ORTHOGONAL);
    cameralist.push_back(lightCam);

    // create the camera draw node, node has been added to the graph in HandleCameraAddition
    GraphNode<DrawGraphNode> * cameraNode = ((CameraSystem*)cameraSystem)->HandleCameraAddition(lightCam, RenderPassTag::DepthPass);
    cameraNode->node->tag = RenderPassTag::DepthPass;
}
*/


void LightSystem::HandleLightAddition(LightAdditionEvent * lightAdditionEvent)
{
    Light* light = lightAdditionEvent->light;

    lightlist.push_back(light);
    light->componentId = GeneratedLightId();
    
    const uint32_t numBindingsInSet = 2; // Light + shadowMap

    ShaderBindingDescription * desc = new ShaderBindingDescription[numBindingsInSet];

    CreateLightUniformDescription(&desc[0], light);
    CreateShadowMap(&desc[1]);

    lightSetWrapper = UniformFactory::GetInstance()->GetSetWrapper(desc, numBindingsInSet);
    CreateLightUniformBuffer(&desc[0], light);
    
    // not creating shadow map image as its the depth attachment from depth pass, already created
    // hence depth image view needs to plugged into the descriptor write

    UniformFactory::GetInstance()->AllocateDescriptorSet(lightSetWrapper, desc, 
        numBindingsInSet, lightUniformAllocConfig.numDescriptorSets);

    // Create sampler 
    // Get the image id
    // Get the View id
    
    // delete ==================
    {
        lightToDescriptionMap.insert(std::pair<Light *, ShaderBindingDescription *>(
        { light, desc }));

        // draw graph node creation
        // top level node as its Set 0
        DrawGraphNode * lightNode = new LightDrawNode();
        lightNode->meshList = MaterialFactory::GetInstance()->GetMeshList(lightSetWrapper, 1);
        lightNode->setLevel = lightSetWrapper->setValue;
        lightNode->setWrapperList.push_back(lightSetWrapper);
        lightNode->tag = RenderPassTag::ColorPass;

        ((LightDrawNode*)lightNode)->descriptorIds = desc->descriptorSetIds;

        GraphNode<DrawGraphNode> * graphNode = new GraphNode<DrawGraphNode>(lightNode);
        lightGraphNodeList.push_back(graphNode);

        DrawGraphManager::GetInstance()->AddNode(graphNode);

        //// create a camera 
        //Camera * lightCam = new Camera(light->GetTransform());
        ////lightCam->SetProjectionType(CameraType::ORTHOGONAL);
        //cameralist.push_back(lightCam);

        //// create the camera draw node, node has been added to the graph in HandleCameraAddition
        //GraphNode<DrawGraphNode> * cameraNode = ((CameraSystem*)cameraSystem)->HandleCameraAddition(lightCam, RenderPassTag::DepthPass);
        //cameraNode->node->tag = RenderPassTag::DepthPass;
    }
    // delete ==================

    CreateLightCamera(light->GetTransform());
}


void LightSystem::HandleMeshAddition(MeshToMatAdditionEvent * meshAdditionEvent)
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

void LightSystem::HandleRendererAddition(MeshRendererAdditionEvent * rendererAdditionEvent)
{
    // whaterever is the material attached to the mesh, we need to check if it casts shadow
    // if yes then add the mesh id to shadowMapping material
    
    uint32_t meshId = rendererAdditionEvent->renderer->geometry->componentId;
    if (!rendererAdditionEvent->renderer->castShadows)
        return;

    if (shadowMappingMat == nullptr)
    {
        ShaderDescription shaders[2];
        shaders[0].type = ShaderType::VERTEX;
        shaders[0].shaderName = "ShadowMapping.vert";

        shaders[1].type = ShaderType::FRAGMENT;
        shaders[1].shaderName = "ShadowMapping.frag";

        shadowMappingMat = MaterialFactory::GetInstance()->CreateMaterial(shaders, 2, meshId, RenderPassTag::DepthPass);
    }
    else
    {
        MaterialFactory::GetInstance()->AddMeshIds(shadowMappingMat, meshId);
    }
}

// wont work as the graphics renderer gets setup before light system subscription for event
void LightSystem::HandleDepthPrepassCreation(DepthPassAttachmentCreationEvent * evt)
{
    
}

void LightSystem::AssignCameraSystem(System * camSystem)
{
    cameraSystem = camSystem;
}

LightSystem::LightSystem()
{
    signature.AddComponent<Light>();
}

LightSystem::~LightSystem()
{
}

