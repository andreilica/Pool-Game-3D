#include "Object2DPool.h"
#include <iostream>
#include <Core/Engine.h>

Mesh* Object2DPool::CreateRectangle(std::string name, glm::vec3 leftBottomCorner, float width, float height, glm::vec3 color, bool fill)
{
	glm::vec3 corner = leftBottomCorner;

	std::vector<VertexFormat> vertices =
	{
		VertexFormat(corner, color),
		VertexFormat(corner + glm::vec3(width, 0, 0), color),
		VertexFormat(corner + glm::vec3(width, height, 0), color),
		VertexFormat(corner + glm::vec3(0, height, 0), color)
	};

	Mesh* rectangle = new Mesh(name);
	std::vector<unsigned short> indices = { 0, 1, 2, 3 };
		
	if (!fill) {
		rectangle->SetDrawMode(GL_LINE_LOOP);
	}
	else {
		// draw 2 triangles. Add the remaining 2 indices
		indices.push_back(0);
		indices.push_back(2);
	}

	rectangle->InitFromData(vertices, indices);
	return rectangle;
}

Mesh* Object2DPool::CreateCircle(std::string name, float radius, glm::vec3 center)
{
	int triangles = 50;
	double k = 0;
	float circleX = center.x;
	float circleZ = center.z;
	float circumference = (float) (2 * M_PI); 

	Mesh* circle = new Mesh(name);
	std::vector<glm::vec3> positions;
	std::vector<glm::vec3> normals;
	std::vector<glm::vec2> textureCoords;
	std::vector<unsigned short> indices;

	positions.push_back(glm::vec3(circleX, 0, circleZ));

	for (double i = 0; i <= triangles; i++) {
		positions.push_back(glm::vec3(circleX + (radius * cos(i * circumference / triangles)), 0, circleZ + (radius * sin(i * circumference / triangles))));
		normals.push_back(glm::vec3(0, -1, 0));
		textureCoords.push_back(glm::vec2(k, k));
		k += 0.02;
	}

	for (int i = 0; i < triangles + 2; i++) {
		indices.push_back(i);
	}

	circle->SetDrawMode(GL_TRIANGLE_FAN);
	circle->InitFromData(positions, normals, textureCoords, indices);

	return circle;
}


Mesh* Object2DPool::CreateDot(std::string name, float radius, glm::vec3 center)
{
	int triangles = 50;
	double k = 0;
	float circleX = center.x;
	float circleZ = center.z;
	float circumference = (float)(2 * M_PI);

	Mesh* circle = new Mesh(name);
	std::vector<glm::vec3> positions;
	std::vector<glm::vec3> normals;
	std::vector<glm::vec2> textureCoords;
	std::vector<unsigned short> indices;

	positions.push_back(glm::vec3(circleX, 0, circleZ));

	for (double i = 0; i <= triangles; i++) {
		positions.push_back(glm::vec3(circleX + (radius * cos(i * circumference / triangles)), 0, circleZ + (radius * sin(i * circumference / triangles))));
		normals.push_back(glm::vec3(0, 1, 0));
		textureCoords.push_back(glm::vec2(k, k));
		k += 0.02;
	}
	normals.push_back(glm::vec3(0, 1, 0));
	textureCoords.push_back(glm::vec2(k, k));
	for (int i = 0; i < triangles + 2; i++) {
		indices.push_back(i);
	}

	circle->SetDrawMode(GL_TRIANGLE_FAN);
	circle->InitFromData(positions, normals, textureCoords, indices);

	return circle;
}