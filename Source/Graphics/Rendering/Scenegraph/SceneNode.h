#pragma once

enum class NODE_TYPE
{
    TRANSFORM,
    GEOMETRY,
    MATERIAL
};

class SceneNode
{
public:
    NODE_TYPE nodeType;

    virtual void Execute() = 0;
    SceneNode(const NODE_TYPE & type) : nodeType(type) {}
};
