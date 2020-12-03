#pragma once

#include "SceneNode.h"

template<typename T>
class TreeNode;

template<typename T>
class TreeNode;

class SceneGraphManager
{

private:
    SceneGraphManager(){}
    SceneGraphManager(SceneGraphManager const &) {}
    SceneGraphManager const & operator= (SceneGraphManager const &) {}

    static SceneGraphManager* instance;
    
public:
    void Init();
    void DeInit();
    void Update();
    static SceneGraphManager* GetInstance();
    ~SceneGraphManager();

    void CreateSceneGraph();
};
