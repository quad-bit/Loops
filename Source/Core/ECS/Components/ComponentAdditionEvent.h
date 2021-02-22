#pragma once

#include <Event.h>
//#include "Camera.h"

class Camera;
class MeshRenderer;
class Light;
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

class LightAdditionEvent : public Event
{
public:
    Light * light;
};

