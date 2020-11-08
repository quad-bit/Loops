#include "Component.h"

#define GLM_ENABLE_EXPERIMENTAL
#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>

namespace Loops::Core::ECS
{
    class Transform : public Component<Transform>
    {
    public:
        glm::vec3 position;
        glm::vec3 scale;
        glm::vec3 eulerAngles;

        glm::mat4 modelMat;
        glm::mat4 translationMat, scaleMat, rotationMat;

        Transform * parent;

        Transform()
        {
            position = glm::vec3(0, 0, 0);
            scale = glm::vec3(1, 1, 1);
            eulerAngles = glm::vec3(0, 0, 0);
            parent = nullptr;
        }
    };
}
