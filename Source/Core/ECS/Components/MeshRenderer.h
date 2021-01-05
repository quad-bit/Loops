#pragma once

#include <Component.h>

class Geometry;
class Material;

class MeshRenderer : public Component<MeshRenderer>
{
public:
    Geometry * geometry;
    Material * material;
};