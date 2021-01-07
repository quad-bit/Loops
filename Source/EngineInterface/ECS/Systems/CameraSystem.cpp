#include "CameraSystem.h"
#include "Camera.h"
#include "ComponentHandle.h"
#include "World.h"
#include "ECS_Setting.h"
#include "Transform.h"
#include "EventBus.h"
#include "ComponentAdditionEvent.h"

void CameraSystem::Init()
{
    EventBus::GetInstance()->Subscribe<CameraSystem, CameraAdditionEvent>(this, &CameraSystem::HandleCameraAddition);
}

void CameraSystem::DeInit()
{
}

void CameraSystem::Update(float dt)
{
    for (auto & entity : registeredEntities)
    {
        //ComponentHandle<Camera> camHandle;
        //worldObj->Unpack(entity, camHandle);

        ComponentHandle<Camera> * camHandle;
        worldObj->Unpack(entity, &camHandle);

        UpdateCameraVectors(camHandle->GetComponent());
    }
}

void CameraSystem::HandleCameraAddition(CameraAdditionEvent * inputEvent)
{
    // recieved the camera addition to the scene 
    float fov = inputEvent->cam->GetFOV();
    // add notify resource manager to add a uniform buffer descriptor set 
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