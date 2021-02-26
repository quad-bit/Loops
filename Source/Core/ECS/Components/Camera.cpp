#include "Camera.h"
#include "Transform.h"

Camera::Camera(Transform * transform, glm::vec3 up, float yaw, float pitch, CameraType projectionType) : movementSpeed(SPEED), mouseSensitivity(SENSITIVTY), fov(FOV)
{
    this->transform = transform;
    //this->position = position;
    //.this->eulerAngles = eulerAngles;
    this->worldUp = up;
    this->yaw = yaw;
    this->pitch = pitch;
    this->aspect = (float)(Settings::windowWidth / Settings::windowHeight);
    this->projectionType = projectionType;
    //this->updateCameraVectors();

    componentType = COMPONENT_TYPE::CAMERA;
}

// Returns the view matrix calculated using Euler Angles and the LookAt Matrix
glm::mat4 Camera::GetViewMatrix()
{
    glm::vec3 localForward = transform->GetForward();
    glm::vec3 localUp = transform->GetUp();
    viewMat = glm::lookAt(transform->GetLocalPosition(), 
        transform->GetLocalPosition() + localForward, 
        localUp );
    return viewMat;
}

glm::mat4 Camera::GetProjectionMat()
{
    glm::mat4 clip = glm::mat4(
        1.0f, 0.0f, 0.0f, 0.0f,
        0.0f, -1.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 0.5f, 0.0f,
        0.0f, 0.0f, 0.5f, 1.0f
    );
    switch (projectionType)
    {
    case CameraType::ORTHOGONAL:
        ASSERT_MSG_DEBUG(0, "Need the correct design");
        break;

    case CameraType::PERSPECTIVE:
        projectionMat = glm::perspective(this->fov, this->aspect, this->zNear, this->zFar);
        break;
    }
    return projectionMat;
}

const glm::vec3* Camera::GetPosition()
{
    return transform->GetLocalPositionRef();
}

glm::vec3 Camera::GetFront()
{
    return transform->GetForward();
}

glm::vec3 Camera::GetUp()
{
    return transform->GetUp();
}

glm::vec3 Camera::GetRight()
{
    return transform->GetRight();
}

glm::vec3& Camera::GetWorlUp()
{
    return this->worldUp;
}

float& Camera::GetYaw()
{
    return this->yaw;
}

float& Camera::GetPitch()
{
    return this->pitch;
}

float& Camera::GetMovementSpeed()
{
    return this->movementSpeed;
}

float& Camera::GetMouseSensitivity()
{
    return this->mouseSensitivity;
}

float& Camera::GetFOV()
{
    return this->fov;
}
