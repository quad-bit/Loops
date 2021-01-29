#pragma once

#include <Component.h>
#include "ECS_Helper.h"

class Mesh;
class Material;


struct TransformUniform
{
    glm::mat4 modelMat;
};


class MeshRenderer : public Component<MeshRenderer>
{
public:
    Mesh * geometry;
    Material * material;

    COMPONENT_TYPE type{COMPONENT_TYPE::MESH_RENDERER};

    MeshRenderer(Mesh * geo, Material * mat)
    {
        geometry = geo;
        material = mat;
    }
};