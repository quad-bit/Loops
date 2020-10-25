#pragma once
#include "System.h"
#include <string>
#include "ComponentHandle.h"
using namespace std;

//class COMPONENTS::MeshFilter;
class Mesh;

class MeshFilterSystem : public System
{
private:
	template <typename t>
	void CreateMesh(ComponentHandle<t>& filterHandle);

public:
	virtual void Init() override;
	virtual void DeInit() override;
	virtual void RegisterEntity(Entity * entity) override;
	virtual void UnRegisterEntity(Entity* entity) override;

	MeshFilterSystem();

	virtual ~MeshFilterSystem();
};

template<typename t>
inline void MeshFilterSystem::CreateMesh(ComponentHandle<t>& filterHandle)
{
	if (filterHandle->meshType == "COMPLEX")
		MeshManagement::MeshManager::GetSingleton()->CreateMesh(filterHandle->meshObj, filterHandle->meshPath, filterHandle->attribMask);
	else
	{
		MeshManagement::MESH_TYPE meshType;
		//"LINE", "TRIANGLE", "PLANE", "CUBE", "SPHERE", "COMPLEX"
		if (filterHandle->meshType == "LINE")
		{
			meshType = MeshManagement::MESH_TYPE::LINE;
		}
		else if (filterHandle->meshType == "TRIANGLE")
		{
			meshType = MeshManagement::MESH_TYPE::TRIANGLE;
		}
		else if (filterHandle->meshType == "PLANE")
		{
			meshType = MeshManagement::MESH_TYPE::PLANE;
		}
		else if (filterHandle->meshType == "CUBE")
		{
			meshType = MeshManagement::MESH_TYPE::CUBE;
			filterHandle->meshObj = new MeshManagement::CubeMesh(filterHandle->attribMask);
		}
		else if (filterHandle->meshType == "SPHERE")
		{
			meshType = MeshManagement::MESH_TYPE::SPHERE;
		}
	}
}
