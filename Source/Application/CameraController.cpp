#include "CameraController.h"
#include "EventBus.h"
#include "InputEvents.h"

void CameraController::Init()
{

}

void CameraController::Update(float dt)
{
    dt = 0.0f;
}

void CameraController::DeInit()
{
}

CameraController::CameraController() : Scriptable(true)
{
    EventBus::GetInstance()->Subscribe(this, &CameraController::MouseDragEventHandler);
}

CameraController::~CameraController()
{
}

void CameraController::MouseDragEventHandler(MouseDragEvent * evt)
{
    int k = 0;
}
