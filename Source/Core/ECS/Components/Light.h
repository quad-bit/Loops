#pragma once
#include <Component.h>
#include <glm\glm.hpp>

class Transform;

enum class LightType
{
    Point,
    Directional,
    SpotLight
};

enum class SpreadEquation
{
    Circle = 0,
    Square = 1,
};

struct LightUniform
{
    glm::vec3 lightPos;
    glm::vec3 lightColor;
};

class Light : public Component<Light>
{
private:
    Transform * transform;

    LightType lightType;

    //valid in case of directional and spot light
    SpreadEquation equation;
    
    float beamRadius;
    float beamWidth;

    float intensity;
    // meant to be used on c++ side to check if the object should be lit or not
    float range; // should be directly proportional to the intensity
    float effectiveRange; // max( intensity * range, range) 

    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;

    Light() = delete;
    Light(const Light &) = delete; 
    Light& operator=(const Light &) = delete;

public:
    Light(Transform * transform, LightType lightType = LightType::Point, 
        SpreadEquation equation = SpreadEquation::Circle)
    {
        this->transform = transform;
        this->lightType = lightType;
        intensity = 0.5f;
        range = 5.0f;
        ambient = glm::vec3(0.2f, 0.2f, 0.2f);
        componentType = COMPONENT_TYPE::LIGHT;
    }
    
    void SetEquationType(const SpreadEquation & equation)
    {
        this->equation = equation;
    }

    void SetRadius(const float & rad)
    {
        this->beamRadius = rad;
    }

    void SetWidth(const float & beamWidth)
    {
        this->beamWidth = beamWidth;
    }

    void SetDiffuse(const glm::vec3 & strength)
    {
        this->diffuse = strength;
    }

    void SetSpecular(const glm::vec3 & strength)
    {
        this->specular = strength;
    }

    void SetRange(const float & range)
    {
        this->range = range;
    }

    void SetIntensity(const float & intensity)
    {
        ASSERT_MSG_DEBUG(intensity > 0.0f && intensity < 1.0f, "intensity should be [0,1]");
        this->intensity = intensity;
    }

    float GetEffectiveRange()
    {
        return glm::max(range, intensity * range);
    }

    Transform * const GetTransform()
    {
        return transform;
    }
};