#pragma once

#define GLM_ENABLE_EXPERIMENTAL
#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>

#include <stdint.h>
#include <iostream>
using namespace std;


struct ComponentCounter {
    static int counter;
};

//int ComponentCounter::counter = 0;

template <typename ComponentType>
class Component
{
private:
	static int family;

public :

    static inline int Family()
    {
        if(family == -1)
			family = ComponentCounter::counter++;

        return family;
    }
};

template <typename ComponentType>
int Component<ComponentType>::family = -1;

//TODO: Need more understanding on the below function

template<typename ComponentType>
int GetComponentFamily()
{
	int family = Component<typename std::remove_const<ComponentType>::type>::Family();
    return family;
}

template<typename ComponentType>
void AssignFamily()
{
	Component<typename std::remove_const<ComponentType>::type>::Family();
}


namespace ComponentSpace
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

class Health : public Component<Health>
{
public:
	int energy;
};

class Material : public Component<Material>
{

};



// =====================================  MeshRenderer
class MeshRenderer : public Component<MeshRenderer>
{

};
// =====================================  MeshRenderer



class Camera : public Component<Camera>
{

};