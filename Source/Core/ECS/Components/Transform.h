#include "Component.h"
#include "SceneNode.h"

#define GLM_ENABLE_EXPERIMENTAL
#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>

struct TransformUniform
{
    glm::mat4 modelMat;
};

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

    glm::vec3 localPosition, globalPosition;
    glm::vec3 localScale, globalScale;
    glm::vec3 localEulerAngle, globalEulerAngle;

    glm::mat4 localModelMatrix;
    glm::mat4 globalModelMatrix;
    glm::mat4 translationMat, scaleMat, rotationMat;

public:

    bool isTransformUpdated;

    Transform(Entity * entity);
    
    Transform * GetParent();
    void SetParent(Transform * transform);
    void UpdateGlobalParams();
    void UpdateLocalParams();
    const std::vector<Transform *> & GetChildren();
    
    glm::vec3 GetLocalPosition();
    glm::vec3 GetLocalEulerAngles();
    glm::vec3 GetLocalScale();
    glm::mat4 GetLocalModelMatrix();

    glm::vec3 GetGlobalPosition();
    glm::vec3 GetGlobalEulerAngles();
    glm::vec3 GetGlobalScale();
    glm::mat4 GetGlobalModelMatrix();
    
    void SetLocalPosition(const glm::vec3 & pos);
    void SetLocalEulerAngles(const glm::vec3 & angle);
    void SetLocalScale(const glm::vec3 & scale);
    void SetLocalModelMatrix(const glm::mat4 & mat);
    
    void SetGlobalPosition(const glm::vec3 & pos);
    void SetGlobalEulerAngles(const glm::vec3 & angle);
    void SetGlobalScale(const glm::vec3 & scale);
    void SetGlobalModelMatrix(const glm::mat4 & mat);

    virtual void Entry() override;
    virtual void Exit() override;

    ~Transform()
    {
    }
};
