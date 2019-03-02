#pragma once

#include <string>

#include <include/glm.h>
#include <Core/GPU/Mesh.h>

namespace Object3DHome
{
	Mesh* CreateMesh(std::string name, const std::vector<VertexFormat> &vertices, const std::vector<unsigned short> &indices);
	Mesh* CreateFrustum(std::string name, float length, float width, float height);
	Mesh* CreateCloth(std::string name, float length, float width, float height);
	Mesh* CreateCuboid(std::string name, float length, float width, float height);
	Mesh* CreateRoom(std::string name, float length, float width, float height);
	Mesh* CreateFloor(std::string name, float length, float width, float height);
	Mesh* CreateCeiling(std::string name, float length, float width, float height);
	Mesh* CreateBank(std::string name, glm::vec3 startCorner, float length, float width, float height, float offset);
	Mesh* CreateRectangle(std::string name, float length, float width);
	Mesh* CreateCylinder(std::string name, float radius, float length);
}

