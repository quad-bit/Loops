#include "MouseInputManager.h"
#include "Assertion.h"
#include "EventBus.h"

MouseInputManager* MouseInputManager::instance = nullptr;

void MouseInputManager::CreateMouseDragEvent()
{
    if (currentMouseButtonDown == MouseButtons::Left)
    {
        MouseDragEvent * evt = FetchMouseDragEvent();
        evt->x = mouseX;
        evt->y = mouseY;

        EventBus::GetInstance()->Publish(evt);
    }
}

void MouseInputManager::Init()
{
    mouseButtonEventPool.resize(buttonEventPoolSize);
    for (uint16_t i = 0; i < buttonEventPoolSize; i++)
    {
        mouseButtonEventPool[i] = new MouseButtonEvent;
    }

    mousePointerEventPool.resize(pointerEventPoolSize);
    for (uint16_t i = 0; i < pointerEventPoolSize; i++)
    {
        mousePointerEventPool[i] = new MousePointerEvent;
    }

    mouseDragEventPool.resize(dragEventPoolSize);
    for (uint16_t i = 0; i < dragEventPoolSize; i++)
    {
        mouseDragEventPool[i] = new MouseDragEvent;
    }
}

void MouseInputManager::DeInit()
{
    for (uint16_t i = 0; i < pointerEventPoolSize; i++)
    {
        delete mousePointerEventPool[i];
    }

    for (uint16_t i = 0; i < buttonEventPoolSize; i++)
    {
        delete mouseButtonEventPool[i];
    }

    for (uint16_t i = 0; i < dragEventPoolSize; i++)
    {
        delete mouseDragEventPool[i];
    }
}

void MouseInputManager::Update()
{

}

MouseInputManager * MouseInputManager::GetInstance()
{
    if (instance == nullptr)
    {
        instance = new MouseInputManager();
    }
    return instance;
}

MouseInputManager::~MouseInputManager()
{
}

MouseButtonEvent * MouseInputManager::FetchMouseButtonEvent()
{
    MouseButtonEvent * evt = mouseButtonEventPool[buttonEventCounter];

    if (buttonEventCounter >= buttonEventPoolSize)
    {
        buttonEventCounter = 0;
    }
    return evt;
}

MousePointerEvent * MouseInputManager::FetchMousePointerEvent()
{
    MousePointerEvent * evt = mousePointerEventPool[pointerEventCounter];

    if (pointerEventCounter >= pointerEventPoolSize)
    {
        pointerEventCounter = 0;
    }
    return evt;
}

MouseDragEvent * MouseInputManager::FetchMouseDragEvent()
{
    MouseDragEvent * evt = mouseDragEventPool[dragEventCounter];

    if (dragEventCounter >= dragEventPoolSize)
    {
        dragEventCounter = 0;
    }
    return evt;
}

void MouseInputManager::MousePointerEventHandler(double x, double y)
{
    //printf("Cursor position: %f %f\n", x, y);
    // publish the event when needed

    mouseX = x;
    mouseY = y;
}

void MouseInputManager::MouseButtonEventHandler(const char * buttonName, const char * buttonEvent)
{
    // publish the event when needed
    const char* upAction = "released";
    const char* pressedAction = "pressed";
    const char* downAction = "repeated";

    MouseButtonEvent * keyEvent = FetchMouseButtonEvent();
    keyEvent->keyname = buttonName;

    if (strcmp(buttonEvent, upAction) == 0)
    {
        keyEvent->keyState = KeyState::RELEASED;
    }
    else if (strcmp(buttonEvent, downAction) == 0)
    {
        keyEvent->keyState = KeyState::DOWN;
    }
    else if (strcmp(buttonEvent, pressedAction) == 0)
    {
        keyEvent->keyState = KeyState::PRESSED;
    }
    currentMouseButtonState = keyEvent->keyState;

    const char* right = "right";
    const char* left = "left";

    currentMouseButtonDown = MouseButtons::None;
    if (strcmp(buttonName, right) == 0)
    {
        currentMouseButtonDown = MouseButtons::Right;
    }
    else if (strcmp(buttonName, left) == 0)
    {
        currentMouseButtonDown = MouseButtons::Left;
    }
    else 
    {
        ASSERT_MSG(0, "Invalid button");
    }

    if (currentMouseButtonDown != MouseButtons::None && 
        keyEvent->keyState == KeyState::DOWN || keyEvent->keyState == KeyState::PRESSED)
    {
        CreateMouseDragEvent();
    }
}
