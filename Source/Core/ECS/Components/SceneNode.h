#pragma once

enum class NODE_TYPE
{
    TRANSFORM,
    GEOMETRY,
    MATERIAL,
    NONE
};

class SceneNode
{
public:
    NODE_TYPE nodeType;
    //SceneNode * parentNode;

    virtual void Execute() = 0;
    SceneNode(const NODE_TYPE & type) : nodeType(type) {}
    SceneNode() { nodeType = NODE_TYPE::NONE; }
};
