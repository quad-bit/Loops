#pragma once

#include <Event.h>
//#include "Camera.h"

class Camera;
class MeshRenderer;
class Entity;

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


