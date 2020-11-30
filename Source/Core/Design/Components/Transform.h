#include "Component.h"

#define GLM_ENABLE_EXPERIMENTAL
#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>

class Transform : public Component<Transform>
{
private:
    Transform * parent;
    std::vector<Transform*> childrenList;


public:
    glm::vec3 localPosition, globalPosition;
    glm::vec3 localScale, globalScale;
    glm::vec3 localEulerAngle, globalEulerAngle;

    glm::mat4 localModelMatrix;
    glm::mat4 globalModelMatrix;
    glm::mat4 translationMat, scaleMat, rotationMat;

    bool isTransformUpdated;

    Transform()
    {
        localPosition = glm::vec3(0, 0, 0);
        localScale = glm::vec3(1, 1, 1);
        localEulerAngle = glm::vec3(0, 0, 0);

        globalPosition = glm::vec3(0, 0, 0);
        globalScale = glm::vec3(1, 1, 1);
        globalEulerAngle = glm::vec3(0, 0, 0);

        parent = nullptr;
    }

    Transform * GetParent();
    void SetParent(Transform * transform);

    const std::vector<Transform *> & GetChildren();

};
