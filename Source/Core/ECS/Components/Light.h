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
    uint8_t isActive;
    uint8_t lightType;
    
    glm::vec3 ligthPos;
    glm::vec3 forward;

    float intensity;
    glm::uvec3 color;
    
    float beamRadius;
    float beamWidth;
    uint8_t equationType;

    glm::uvec3 ambient;
    glm::uvec3 diffuse;
    glm::uvec3 specular;
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

    glm::uvec3 ambient;
    glm::uvec3 diffuse;
    glm::uvec3 specular;

public:
    Light(Transform * transform, LightType type = LightType::Point, 
        SpreadEquation equation = SpreadEquation::Circle)
    {
        this->transform = transform;
        lightType = type;
        intensity = 0.5f;
        range = 5.0f;
        ambient = glm::uvec3(0.2f, 0.2f, 0.2f);
    }
    
    void SetEquationType(const SpreadEquation & equation)
    {
        this->equation = equation;
    }

    void SetRadius(const float & rad)
    {
        this->beamRadius = rad;
    }

    void SetWidth(const uint32_t & beamWidth)
    {
        this->beamWidth = beamWidth;
    }

    void SetDiffuse(const glm::uvec3 & strength)
    {
        this->diffuse = strength;
    }

    void SetSpecular(const glm::uvec3 & strength)
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
};