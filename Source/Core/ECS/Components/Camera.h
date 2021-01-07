#pragma once

#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <Settings.h>
#include "Assertion.h"

// Defines several possible options for camera movement. Used as abstraction to stay away from window-system specific input methods
enum class Camera_Movement
{
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT
};

// Default camera values
const float YAW = -90.0f;
const float PITCH = 0.0f;
const float SPEED = 6.0f;
const float SENSITIVTY = 0.25f;
const float FOV = 45.0f;

enum class CameraType
{
    PERSPECTIVE,
    ORTHOGONAL
};

// An abstract camera class that processes input and calculates the corresponding Eular Angles, Vectors and Matrices for use in OpenGL
class Camera
{
private:
    // Camera Attributes
    glm::vec3* position;

    // Directions
    glm::vec3 front;
    glm::vec3 up;
    glm::vec3 right;
    glm::vec3 worldUp;
    glm::mat4 viewMat;
    glm::mat4 projectionMat;

    // Eular Angles
    float yaw;
    float pitch;

    // Camera options
    float movementSpeed;
    float mouseSensitivity;
    float fov;
    float aspect;
    float zNear = 1.0f, zFar = 1000.0f;
    CameraType projectionType;

    void * cameraDataRaw;

public:
    // Constructor with vectors
    Camera(glm::vec3 * position, glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), float yaw = YAW, float pitch = PITCH, CameraType projectionType = CameraType::PERSPECTIVE) : front(glm::vec3(0.0f, 0.0f, -1.0f)), movementSpeed(SPEED), mouseSensitivity(SENSITIVTY), fov(FOV)
    {
        this->position = position;
        this->worldUp = up;
        this->yaw = yaw;
        this->pitch = pitch;
        this->aspect = (float)(Settings::windowWidth / Settings::windowHeight);
        this->projectionType = projectionType;
        //this->updateCameraVectors();
    }

    //// Constructor with scalar values
    //Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch, CameraType projectionType) : front(glm::vec3(0.0f, 0.0f, -1.0f)), movementSpeed(SPEED), mouseSensitivity(SENSITIVTY), fov(FOV)
    //{
    //    this->position = glm::vec3(posX, posY, posZ);
    //    this->worldUp = glm::vec3(upX, upY, upZ);
    //    this->yaw = yaw;
    //    this->pitch = pitch;
    //    this->projectionType = projectionType;
    //    this->aspect = (float)(Settings::windowWidth / Settings::windowHeight);

    //    //this->updateCameraVectors();
    //}

    // Returns the view matrix calculated using Eular Angles and the LookAt Matrix
    glm::mat4 GetViewMatrix()
    {
        viewMat = glm::lookAt(*this->position, *this->position + this->front, this->up);
        return viewMat;
    }

    glm::mat4 GetProjectionMat()
    {
        switch (projectionType)
        {
        case CameraType::ORTHOGONAL:
            ASSERT_MSG(0, "Need the correct design");
            break;

        case CameraType::PERSPECTIVE:
            projectionMat = glm::perspective(this->fov, this->aspect, this->zNear, this->zFar);
            break;
        }
        return projectionMat;
    }

    const glm::vec3* GetPosition()
    {
        return this->position;
    }

    glm::vec3& GetFront()
    {
        return this->front;
    }

    glm::vec3& GetUp()
    {
        return this->up;
    }

    glm::vec3& GetRight()
    {
        return this->right;
    }

    glm::vec3& GetWorlUp()
    {
        return this->worldUp;
    }

    float& GetYaw()
    {
        return this->yaw;
    }

    float& GetPitch()
    {
        return this->pitch;
    }

    float& GetMovementSpeed()
    {
        return this->movementSpeed;
    }

    float& GetMouseSensitivity()
    {
        return this->mouseSensitivity;
    }

    float& GetFOV()
    {
        return this->fov;
    }
};