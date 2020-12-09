#include "Component.h"
#include "SceneNode.h"

#define GLM_ENABLE_EXPERIMENTAL
#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>


class Entity;
class Transform : public Component<Transform>, public SceneNode
{
private:
    Transform * parent;
    std::vector<Transform*> childrenList;

    Entity * owner;
    string * entityName;

    void Init();
    Transform();

public:

    glm::vec3 localPosition, globalPosition;
    glm::vec3 localScale, globalScale;
    glm::vec3 localEulerAngle, globalEulerAngle;

    glm::mat4 localModelMatrix;
    glm::mat4 globalModelMatrix;
    glm::mat4 translationMat, scaleMat, rotationMat;

    bool isTransformUpdated;

    Transform(Entity * entity);
    
    Transform * GetParent();
    void SetParent(Transform * transform);

    const std::vector<Transform *> & GetChildren();

    virtual void Execute() override;

    ~Transform()
    {
    }
};
