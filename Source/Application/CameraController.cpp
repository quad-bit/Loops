#include "CameraController.h"
#include "EventBus.h"
#include "InputEvents.h"
#include "Entity.h"
#include "Transform.h"
#include "glm\glm.hpp"
#include "EntityHandle.h"
#include "Camera.h"

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
    int k = 0;
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
            glm::vec3 pos = cam->GetFront() * velocity + trf->GetLocalPosition();// trf->GetLocalPosition() + glm::vec3(0, 0, 1);
            trf->SetLocalPosition(pos);
        }
        else if (strcmp(a, evt->keyname) == 0)
        {
            glm::vec3 pos = -cam->GetRight() * velocity + trf->GetLocalPosition();// trf->GetLocalPosition() + glm::vec3(0, 0, 1);
            trf->SetLocalPosition(pos);
        }
        else if (strcmp(s, evt->keyname) == 0)
        {
            glm::vec3 pos = -cam->GetFront() * velocity + trf->GetLocalPosition();// trf->GetLocalPosition() + glm::vec3(0, 0, 1);
            trf->SetLocalPosition(pos);
        }
        else if (strcmp(d, evt->keyname) == 0)
        {
            glm::vec3 pos = cam->GetRight() * velocity + trf->GetLocalPosition();// trf->GetLocalPosition() + glm::vec3(0, 0, 1);
            trf->SetLocalPosition(pos);
        }
    }
        break;
    }
}
