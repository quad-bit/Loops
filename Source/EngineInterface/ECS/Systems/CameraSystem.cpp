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
//#include "Settings.h"

uint32_t CameraSystem::GetCamId()
{
    return idCounter++;
}

size_t CameraSystem::GetDataSizeMeantForSharing()
{
    return sizeof(CameraUniform) * allocConfig.numDescriptors * resourceSharingConfig.maxUniformPerResource;
}

std::vector<size_t> CameraSystem::CalculateOffsetsForDescInUniform(size_t dataSizePerDescriptor)
{
    std::vector<size_t> offsets;
    offsets.resize(allocConfig.numDescriptors);

    uint32_t index = resourceSharingConfig.allocatedUniformCount % resourceSharingConfig.maxUniformPerResource;

    for (uint32_t i = 0; i < allocConfig.numDescriptors; i++)
    {
        offsets[i] = index * allocConfig.numDescriptors * dataSizePerDescriptor + dataSizePerDescriptor * i;
    }
    return offsets;
}

bool CameraSystem::IsNewAllocationRequired()
{
    if (resourceSharingConfig.allocatedUniformCount % resourceSharingConfig.maxUniformPerResource == 0)
        return true;
    else
        return false;
}

void CameraSystem::Init()
{
    EventBus::GetInstance()->Subscribe<CameraSystem, CameraAdditionEvent>(this, &CameraSystem::HandleCameraAddition);
    
    allocConfig.numDescriptors = Settings::maxFramesInFlight;
    allocConfig.numMemories = 1;
    allocConfig.numResources = 1;

    resourceSharingConfig.maxUniformPerResource = 2;
    resourceSharingConfig.allocatedUniformCount = 0;
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

        // TODO : write the uniform data of Camera to gpu memory via void*
    }
}

void CameraSystem::HandleCameraAddition(CameraAdditionEvent * inputEvent)
{
    // recieved the camera addition to the scene 
    cameraList.push_back(inputEvent->cam);
    inputEvent->cam->componentId = GetCamId();

    ShaderBindingDescription * desc = new ShaderBindingDescription;
    desc->set = 0;
    desc->binding = 0;
    desc->numElements = 2;
    desc->resourceName = "View";
    desc->resourceType = DescriptorType::UNIFORM_BUFFER;
    desc->resParentId = inputEvent->cam->componentId;
    desc->parentType = inputEvent->cam->type;
    desc->dataSizePerDescriptor = sizeof(CameraUniform);
    desc->uniformId = inputEvent->cam->componentId; // as one cam = one uniform
    desc->offsetsForEachDescriptor = CalculateOffsetsForDescInUniform(sizeof(CameraUniform));
    desc->allocationConfig = allocConfig;

    // Check if it can be fit into an existing buffer
    if (IsNewAllocationRequired())
    {
        size_t totalSize = GetDataSizeMeantForSharing();
        // True : Allocate new buffer
        UniformFactory::GetInstance()->AllocateResource(desc, &totalSize,  1, AllocationMethod::LAZY);
    }
    else
    {
        // False : Assign the buffer id to this shaderResourceDescription
        desc->resourceId = resDescriptionList[resDescriptionList.size() - 1]->resourceId;
    }

    resDescriptionList.push_back(desc);
    resourceSharingConfig.allocatedUniformCount += 1;

    CameraUniform obj = {};
    obj.projectionMat = inputEvent->cam->GetProjectionMat();
    obj.viewMat = inputEvent->cam->GetViewMatrix();

    //upload data to buffers
    for(uint32_t i = 0; i < Settings::maxFramesInFlight; i++)
    {
        UniformFactory::GetInstance()->UploadDataToBuffers(desc->resourceId, desc->dataSizePerDescriptor, &obj, desc->offsetsForEachDescriptor[i], false);
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