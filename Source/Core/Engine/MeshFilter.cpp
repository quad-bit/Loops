#include "MeshFilter.h"
#include <Mesh.h>
#include <VulkanObjectSpace\VertexBuffer.h>
#include <VulkanObjectSpace\IndexBuffer.h>

using namespace VulkanObjectSpace;

using namespace MeshManagement;
using namespace ComponentSpace;

string ComponentSpace::MeshFilter::availableMeshTypes[6] { "LINE", "TRIANGLE", "PLANE", "CUBE", "SPHERE", "COMPLEX" };


MeshFilter::MeshFilter()
{
	attribMask = uint8_t{ 0b000000111 };

	//availableMeshTypes = { "LINE", "TRIANGLE", "PLANE", "CUBE", "SPHERE", "COMPLEX" };
	//////1:position, 2:colors, 3:normals, 4:uvs
}

MeshFilter::MeshFilter(string& meshType, bool hasNormals, bool hasUVs, bool hasColors)
{
	
	uint8_t uvMask = 1 << (hasUVs ? 0 : 3);// { 0b00001000 };
	uint8_t normMask = 1 << (hasUVs ? 0 : 2); //{ 0b00000100 };
	uint8_t colMask = 1 << (hasUVs ? 0 : 1); //{ 0b00000010 };

	attribMask |= (uvMask | colMask | normMask);
}

MeshFilter::MeshFilter(bool hasNormals, bool hasUVs, bool hasColors, string& meshPath)
{
	uint8_t uvMask = 1 << (hasUVs ? 0 : 3);// { 0b00001000 };
	uint8_t normMask = 1 << (hasUVs ? 0 : 2); //{ 0b00000100 };
	uint8_t colMask = 1 << (hasUVs ? 0 : 1); //{ 0b00000010 };

	attribMask |= (uvMask | colMask | normMask);
}

MeshFilter::MeshFilter(MeshFilter const & obj)
{
	this->hasColors = obj.hasColors;
	this->meshObj = obj.meshObj;
}

MeshFilter::~MeshFilter()
{
	delete indexBufferObj;
	delete vertexBufferObj;
	delete meshObj; 
}