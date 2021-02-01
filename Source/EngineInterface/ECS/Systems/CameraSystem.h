#pragma once
#include "System.h"
#include <glm\glm.hpp>
#include "ShaderResourceDescription.h"
#include "DrawGraphNode.h"
#include "MeshAdditionEvent.h"

template <typename T>
class GraphNode;

class Camera;
enum class Camera_Movement;
class CameraAdditionEvent;

template <typename T>
class ComponentHandle;

class CameraGraphNode : public DrawGraphNode
{
public:
    virtual void Execute() override;
    CameraGraphNode()
    {
        drawNodeType = DrawNodeTypes::CAMERA;
    }
};

class CameraSystem : public System
{
private:
    // Processes input received from any keyboard-like input system. Accepts input parameter in the form of camera defined ENUM (to abstract it from windowing systems)
    void ProcessKeyboard(Camera * cam, glm::vec3 * camTransformPos, Camera_Movement * direction, float deltaTime);

    // Processes input received from a mouse input system. Expects the offset value in both the x and y direction.
    void ProcessMouseMovement(Camera * cam, float xOffset, float yOffset, bool constrainPitch = true);
    
    // Processes input received from a mouse scroll-wheel event. Only requires input on the vertical wheel-axis
    void ProcessMouseScroll(float yOffset) {}
  
    // Calculates the front vector from the Camera's (updated) Eular Angles
    void UpdateCameraVectors(Camera * cam);

    std::vector<Camera *> cameraList;
    std::vector<ShaderBindingDescription *> resDescriptionList;
    GlobalResourceAllocationConfig allocConfig;
    GlobalResourceSharingConfig resourceSharingConfig;

    uint32_t idCounter = 0;
    uint32_t GeneratedCamId();
    //size_t GetDataSizeMeantForSharing();
    //std::vector<size_t> CalculateOffsetsForDescInUniform(size_t dataSizePerDescriptor);
    //bool IsNewAllocationRequired();

    SetWrapper * cameraSetWrapper;

    std::vector<GraphNode<DrawGraphNode> *> cameraGraphNodeList;

public:
    virtual void Init() override;
    virtual void DeInit() override;
    virtual void Update(float dt) override;

    void HandleCameraAddition(CameraAdditionEvent * inputEvent);
    void HandleMeshAddition(MeshAdditionEvent *  meshAdditionEvent);

    CameraSystem();
    virtual ~CameraSystem();
};
