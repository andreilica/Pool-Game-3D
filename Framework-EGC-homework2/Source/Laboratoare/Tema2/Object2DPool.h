#pragma once

#include <string>

#include <include/glm.h>
#include <Core/GPU/Mesh.h>

namespace Object2DPool
{
	Mesh* CreateRectangle(std::string name, glm::vec3 leftBottomCorner, float width, float height, glm::vec3 color, bool fill = false);
	Mesh* CreateCircle(std::string name, float radius, glm::vec3 center);
	Mesh* CreateDot(std::string name, float radius, glm::vec3 center);
}

