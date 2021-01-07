#pragma once

#include <Event.h>
#include "Camera.h"

struct CameraAdditionEvent : public Event
{
    Camera * cam;
};

//
//struct NodeDeletionEvent : public Event
//{
//    SceneNode * node;
//
//};
