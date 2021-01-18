#pragma once

#include <Component.h>
#include "ECS_Helper.h"

class Geometry;
class Material;

class MeshRenderer : public Component<MeshRenderer>
{
public:
    Geometry * geometry;
    Material * material;

    COMPONENT_TYPE type{ COMPONENT_TYPE::MESH_RENDERER};
};