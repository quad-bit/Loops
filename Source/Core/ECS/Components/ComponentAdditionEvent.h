#pragma once

#include <Event.h>
#include "Camera.h"

class MeshRenderer;
class CameraAdditionEvent : public Event
{
public:
    Camera * cam;
};

class MeshRendererAdditionEvent : public Event
{
public:
    MeshRenderer * renderer;
};


