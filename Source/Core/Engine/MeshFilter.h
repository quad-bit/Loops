#pragma once

#include "Component.h"
#include <string>
//#include <Mesh.h>

namespace MeshManagement
{
	class Mesh;
}

namespace VulkanObjectSpace
{
	class VertexBuffer;
	class IndexBuffer;
}

namespace ComponentSpace
{
	class MeshFilter : public Component<MeshFilter>
	{
	private:

	public :
		MeshManagement::Mesh * meshObj;
		VulkanObjectSpace::VertexBuffer * vertexBufferObj;
		VulkanObjectSpace::IndexBuffer * indexBufferObj;
		
		uint8_t attribMask{ 0b00000001 };
		static string availableMeshTypes[6];// { "LINE", "TRIANGLE", "PLANE", "CUBE", "SPHERE", "COMPLEX" };
		string meshType = "";
		string meshPath = "";
		bool hasNormals;
		bool hasUVs;
		bool hasColors;

		MeshFilter();
		MeshFilter(string& meshType, bool hasNormals, bool hasUVs, bool hasColors);
		MeshFilter(bool hasNormals, bool hasUVs, bool hasColors, string& meshPath);
		MeshFilter(MeshFilter const &);

		virtual ~MeshFilter();
	};
	
}
