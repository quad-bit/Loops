#pragma once

#include <Event.h>
#include "Camera.h"

class CameraAdditionEvent : public Event
{
public:
    Camera * cam;
};


