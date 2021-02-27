#include "CameraSystem.h"
#include "Camera.h"
#include "ComponentHandle.h"
#include "World.h"
#include "ECS_Setting.h"
#include "Transform.h"
#include "EventBus.h"
#include "ComponentAdditionEvent.h"
#include "ShaderResourceDescription.h"
#include "UniformFactory.h"
#include "ResourceAllocationHelper.h"
#include "MaterialFactory.h"
#include "DrawGraphManager.h"
#include "DrawCommandBuffer.h"
#include "DrawGraphNode.h"

uint32_t CameraSystem::GeneratedCamId()
{
    return idCounter++;
}

void CameraSystem::Init()
{
    EventBus::GetInstance()->Subscribe<CameraSystem, CameraAdditionEvent>(this, &CameraSystem::HandleCameraAddition);
    EventBus::GetInstance()->Subscribe<CameraSystem, MeshAdditionEvent>(this, &CameraSystem::HandleMeshAddition);

    allocConfig.numDescriptors = Settings::maxFramesInFlight;
    allocConfig.numMemories = 1;
    allocConfig.numResources = 1;

    resourceSharingConfig.maxUniformPerResource = 2;
    resourceSharingConfig.allocatedUniformCount = 0;

    size_t uniformSize = sizeof(CameraUniform);
    memoryAlignedUniformSize = UniformFactory::GetInstance()->GetMemoryAlignedDataSizeForBuffer(uniformSize);
}

void CameraSystem::DeInit()
{
    for (uint32_t i = 0; i < resDescriptionList.size(); i++)
    {
        delete resDescriptionList[i];
    }
    resDescriptionList.clear();
}

void CameraSystem::Update(float dt)
{
    for (auto & entity : registeredEntities)
    {
        ComponentHandle<Camera> * camHandle;
        worldObj->Unpack(entity, &camHandle);

        UpdateCameraVectors(camHandle->GetComponent());

        CameraUniform obj = {};
        obj.projectionMat = camHandle->GetComponent()->GetProjectionMat();
        obj.viewMat = camHandle->GetComponent()->GetViewMatrix();
        obj.cameraPos = *camHandle->GetComponent()->GetPosition();

        ShaderBindingDescription * desc = camToDescriptionMap[camHandle->GetComponent()];
        //upload data to buffers
        //for (uint32_t i = 0; i < allocConfig.numDescriptors; i++)
        {
            UniformFactory::GetInstance()->UploadDataToBuffers(desc->resourceId, desc->dataSizePerDescriptorAligned, memoryAlignedUniformSize, &obj, desc->offsetsForEachDescriptor[Settings::currentFrameInFlight], false);
        }

        // TODO : write the uniform data of Camera to gpu memory via void*
    }
}

void CameraSystem::HandleCameraAddition(CameraAdditionEvent * inputEvent)
{
    // recieved the camera addition to the scene 
    cameraList.push_back(inputEvent->cam);
    inputEvent->cam->componentId = GeneratedCamId();

    ShaderBindingDescription * desc = new ShaderBindingDescription;
    desc->set = (uint32_t)ResourceSets::CAMERA;
    desc->binding = 0;
    desc->numElements = 3;
    desc->resourceName = "View";
    desc->resourceType = DescriptorType::UNIFORM_BUFFER;
    desc->resParentId = inputEvent->cam->componentId;
    desc->parentType = inputEvent->cam->componentType;
    desc->dataSizePerDescriptorAligned = memoryAlignedUniformSize;
    desc->uniformId = inputEvent->cam->componentId; // as one cam = one uniform
    desc->offsetsForEachDescriptor = AllocationUtility::CalculateOffsetsForDescInUniform(memoryAlignedUniformSize, allocConfig, resourceSharingConfig);
    desc->allocationConfig = allocConfig;

    // Check if it can be fit into an existing buffer
    if (AllocationUtility::IsNewAllocationRequired(resourceSharingConfig))
    {
        size_t totalSize = AllocationUtility::GetDataSizeMeantForSharing(memoryAlignedUniformSize, allocConfig, resourceSharingConfig);
        // True : Allocate new buffer
        cameraSetWrapper = UniformFactory::GetInstance()->AllocateResource(desc, &totalSize,  1, AllocationMethod::LAZY);
    }
    else
    {
        // False : Assign the buffer id to this shaderResourceDescription
        desc->resourceId = resDescriptionList[resDescriptionList.size() - 1]->resourceId;
        desc->resourceMemoryId = resDescriptionList[resDescriptionList.size() - 1]->resourceMemoryId;
    }

    resDescriptionList.push_back(desc);
    resourceSharingConfig.allocatedUniformCount += 1;

    CameraUniform obj = {};
    obj.projectionMat = inputEvent->cam->GetProjectionMat();
    obj.viewMat = inputEvent->cam->GetViewMatrix();
    obj.cameraPos = *inputEvent->cam->GetPosition();

    //upload data to buffers
    for(uint32_t i = 0; i < allocConfig.numDescriptors; i++)
    {
        UniformFactory::GetInstance()->UploadDataToBuffers(desc->resourceId, sizeof(CameraUniform), memoryAlignedUniformSize, &obj, desc->offsetsForEachDescriptor[i], false);
    }

    UniformFactory::GetInstance()->AllocateDescriptors(cameraSetWrapper, desc, 1, allocConfig.numDescriptors);

    camToDescriptionMap.insert(std::pair<Camera *, ShaderBindingDescription *>(
    { inputEvent->cam , desc}));

    // draw graph node creation
    // top level node as its Set 0
    DrawGraphNode * cameraNode = new CameraDrawNode();
    cameraNode->meshList = MaterialFactory::GetInstance()->GetMeshList(cameraSetWrapper, 1);
    cameraNode->setLevel = cameraSetWrapper->setValue;
    cameraNode->setWrapperList.push_back(cameraSetWrapper);
    ((CameraDrawNode*)cameraNode)->descriptorIds = desc->descriptorIds;

    GraphNode<DrawGraphNode> * graphNode = new GraphNode<DrawGraphNode>(cameraNode);
    cameraGraphNodeList.push_back(graphNode);

    //nodeToDescriptionMap.insert(std::pair<DrawGraphNode*, ShaderBindingDescription *>
    //    ({cameraNode, desc}));

    DrawGraphManager::GetInstance()->AddNode(graphNode);
}

void CameraSystem::HandleMeshAddition(MeshAdditionEvent * meshAdditionEvent)
{
    uint32_t setCount = (uint32_t)meshAdditionEvent->setWrapperList.size();
    for (uint32_t k = 0; k < cameraGraphNodeList.size(); k++)
    for (uint32_t i = 0; i < setCount; i++)
    {
        SetWrapper * wrapper = meshAdditionEvent->setWrapperList[i];
        if (wrapper->id == cameraSetWrapper->id)
        {
            std::vector<uint32_t> meshList = cameraGraphNodeList[k]->node->meshList;
            std::vector<uint32_t>::iterator it;
            it = std::find_if(meshList.begin(), meshList.end(), [=](uint32_t id) {
                return id == meshAdditionEvent->meshId; });

            if (it == meshList.end())
            {
                cameraGraphNodeList[k]->node->meshList.push_back(meshAdditionEvent->meshId);
            }
        }
    }
}

CameraSystem::CameraSystem()
{
    signature.AddComponent<Camera>();
    signature.AddComponent<Transform>();
}

CameraSystem::~CameraSystem()
{
}

void CameraSystem::ProcessKeyboard(Camera * cam, glm::vec3 * camTransformPos, Camera_Movement * direction, float deltaTime)
{
    float velocity = cam->GetMovementSpeed()* deltaTime;

    if (*direction == Camera_Movement::FORWARD)
    {
        *camTransformPos += cam->GetFront() * velocity;
    }

    if (*direction == Camera_Movement::BACKWARD)
    {
        *camTransformPos -= cam->GetFront() * velocity;
    }

    if (*direction == Camera_Movement::LEFT)
    {
        *camTransformPos -= cam->GetRight() * velocity;
    }

    if (*direction == Camera_Movement::RIGHT)
    {
        *camTransformPos += cam->GetRight() * velocity;
    }
}

void CameraSystem::ProcessMouseMovement(Camera * cam, float xOffset, float yOffset, bool constrainPitch)
{
    xOffset *= cam->GetMouseSensitivity();
    yOffset *= cam->GetMouseSensitivity();

    cam->GetYaw() += xOffset;
    cam->GetPitch() += yOffset;

    // Make sure that when GetPitch() is out of bounds, screen doesn't get flipped
    if (constrainPitch)
    {
        if (cam->GetPitch() > 89.0f)
        {
            cam->GetPitch() = 89.0f;
        }

        if (cam->GetPitch() < -89.0f)
        {
            cam->GetPitch() = -89.0f;
        }
    }

    // Update Front, Right and Up Vectors using the updated Eular angles
    //cam->updateCameraVectors();
    UpdateCameraVectors(cam);
}

void CameraSystem::UpdateCameraVectors(Camera * cam)
{
    // Calculate the new Front vector
    glm::vec3 front;
    front.x = cos(glm::radians(cam->GetYaw())) * cos(glm::radians(cam->GetPitch()));
    front.y = sin(glm::radians(cam->GetPitch()));
    front.z = sin(glm::radians(cam->GetYaw())) * cos(glm::radians(cam->GetPitch()));
    cam->GetFront() = glm::normalize(front);
    // Also re-calculate the Right and Up vector
    cam->GetRight() = glm::normalize(glm::cross(cam->GetFront(), cam->GetWorlUp()));  // Normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
    cam->GetUp() = glm::normalize(glm::cross(cam->GetRight(), cam->GetFront()));
}

