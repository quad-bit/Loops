#include "CameraController.h"
#include "EventBus.h"
#include "InputEvents.h"
#include "Entity.h"
#include "Transform.h"
#include "glm\glm.hpp"
#include "MathUtil.h"
#include "EntityHandle.h"
#include "Camera.h"
#include <CorePrecompiled.h>

void CameraController::RotateCamera(glm::vec2 mousePosition)
{
}

void CameraController::Activated()
{
    cam = entityHandle->GetComponent<Camera>().GetComponent();
}

void CameraController::Init()
{
}

void CameraController::Update(float dt)
{
    
}

void CameraController::DeInit()
{
}

CameraController::CameraController() : Scriptable(true)
{
    EventBus::GetInstance()->Subscribe(this, &CameraController::KeyBoardEventHandler);
    EventBus::GetInstance()->Subscribe(this, &CameraController::MouseDragEventHandler);
}

CameraController::~CameraController()
{
}

void CameraController::MouseDragEventHandler(MouseDragEvent * evt)
{
    glm::vec2 mouseScreenCoord = glm::vec2(evt->x, evt->y);
    
    if (mouseScreenCoord.x > 0 && mouseScreenCoord.x < Settings::windowWidth &&
        mouseScreenCoord.y > 0 && mouseScreenCoord.y < Settings::windowHeight)
    {
          
        float x = MathUtil::RangeConversion<float>(mouseScreenCoord.x / Settings::windowWidth, -1, 1);
        float y = -MathUtil::RangeConversion<float>(mouseScreenCoord.y / Settings::windowHeight, -1, 1);
        glm::vec3 ndc = glm::vec3(x, y, -1);
        glm::vec4 transformed = glm::inverse<4, 4, float, glm::qualifier::highp>(cam->GetProjectionMat()) *
            glm::inverse<4, 4, float, glm::qualifier::highp>(cam->GetViewMatrix()) * Vec3ToVec4_1(ndc);
        PLOGD << "Pressed " << transformed.x * 10 << "  " << transformed.y * 10;
        
    }
}

void CameraController::KeyBoardEventHandler(KeyInputEvent * evt)
{
    const char * keyname = evt->keyname;

    const char * w = "W";
    const char * a = "A";
    const char * s = "S";
    const char * d = "D";

    switch (evt->keyState)
    {
    case KeyState::DOWN:
    case KeyState::PRESSED:
        {
            Transform * trf = entityHandle->GetTransform();
            //move the camera
            if (strcmp(w, evt->keyname) == 0)
            {
                glm::vec3 pos = cam->GetFront() * velocity + trf->GetLocalPosition();
                trf->SetLocalPosition(pos);
            }
            else if (strcmp(a, evt->keyname) == 0)
            {
                glm::vec3 pos = -cam->GetRight() * velocity + trf->GetLocalPosition();
                trf->SetLocalPosition(pos);
            }
            else if (strcmp(s, evt->keyname) == 0)
            {
                glm::vec3 pos = -cam->GetFront() * velocity + trf->GetLocalPosition();
                trf->SetLocalPosition(pos);
            }
            else if (strcmp(d, evt->keyname) == 0)
            {
                glm::vec3 pos = cam->GetRight() * velocity + trf->GetLocalPosition();
                trf->SetLocalPosition(pos);
            }
        }
        break;
    }
}
