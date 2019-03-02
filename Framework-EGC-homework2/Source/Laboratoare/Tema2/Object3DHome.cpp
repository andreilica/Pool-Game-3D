#include "Object3DHome.h"
#include <iostream>
#include <Core/Engine.h>


Mesh* Object3DHome::CreateMesh(std::string name, const std::vector<VertexFormat> &vertices, const std::vector<unsigned short> &indices)
{
	unsigned int VAO = 0;
	// TODO: Create the VAO and bind it
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	// TODO: Create the VBO and bind it
	unsigned int VBO;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	// TODO: Send vertices data into the VBO buffer
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices[0]) * vertices.size(), &vertices[0], GL_STATIC_DRAW);

	// TODO: Crete the IBO and bind it
	unsigned int IBO;
	glGenBuffers(1, &IBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);

	// TODO: Send indices data into the IBO buffer
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices[0]) * indices.size(), &indices[0], GL_STATIC_DRAW);

	// ========================================================================
	// This section describes how the GPU Shader Vertex Shader program receives data

	// set vertex position attribute
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), 0);

	// set vertex normal attribute
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), (void*)(sizeof(glm::vec3)));

	// set texture coordinate attribute
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), (void*)(2 * sizeof(glm::vec3)));

	// set vertex color attribute
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), (void*)(2 * sizeof(glm::vec3) + sizeof(glm::vec2)));
	// ========================================================================

	// Unbind the VAO
	glBindVertexArray(0);

	// Check for OpenGL errors
	CheckOpenGLError();

	// Mesh information is saved into a Mesh object
	Mesh* object = new Mesh(name);
	object->InitFromBuffer(VAO, static_cast<unsigned short>(indices.size()));
	object->vertices = vertices;
	object->indices = indices;
	return object;
}

Mesh* Object3DHome::CreateCloth(std::string name, float length, float width, float height)
{
	float offSet = 0.3f;
	glm::vec3 startCorner = glm::vec3(0, 0, 0.71f);
	
	std::vector<glm::vec3> positions
	{
		startCorner + glm::vec3(-offSet, height, offSet),
		startCorner + glm::vec3(length + offSet, height, offSet),
		startCorner + glm::vec3(-offSet, height, -width - offSet),
		startCorner + glm::vec3(length + offSet, height, -width - offSet),
	};

	std::vector<glm::vec3> normals
	{
		glm::vec3(0, 1, 0),
		glm::vec3(0, 1, 0),
		glm::vec3(0, 1, 0),
		glm::vec3(0, 1, 0),
	};

	std::vector<glm::vec2> textureCoords
	{
		glm::vec2(0, 0),
		glm::vec2(1, 0),
		glm::vec2(0, 1),
		glm::vec2(1, 1),
	};

	std::vector<unsigned short> indices =
	{
		0, 1, 3,
		0, 3, 2
	};

	Mesh* object = new Mesh(name);
	object->InitFromData(positions, normals, textureCoords, indices);
	return object;
}

Mesh* Object3DHome::CreateRectangle(std::string name, float length, float width)
{
	glm::vec3 startCorner = glm::vec3(0, 0, 0.71f);

	std::vector<glm::vec3> positions
	{
		startCorner,
		startCorner + glm::vec3(length, 0, 0),
		startCorner + glm::vec3(0, width, 0),
		startCorner + glm::vec3(length, width, 0),
	};

	std::vector<glm::vec3> normals
	{
		glm::vec3(0, 0, 1),
		glm::vec3(0, 0, 1),
		glm::vec3(0, 0, 1),
		glm::vec3(0, 0, 1),
	};

	std::vector<glm::vec2> textureCoords
	{
		glm::vec2(0, 0),
		glm::vec2(1, 0),
		glm::vec2(0, 1),
		glm::vec2(1, 1),
	};

	std::vector<unsigned short> indices =
	{
		0, 1, 3,
		0, 3, 2
	};

	Mesh* object = new Mesh(name);
	object->InitFromData(positions, normals, textureCoords, indices);
	return object;
}

Mesh* Object3DHome::CreateFrustum(std::string name, float length, float width, float height)
{

	glm::vec3 startCorner = glm::vec3(0, 0, 0.71f);
	float offSet = 0.3f;
	std::vector<glm::vec3> positions
	{
		startCorner,
		startCorner + glm::vec3(length, 0,  0),
		startCorner + glm::vec3(-offSet, height, offSet),
		startCorner + glm::vec3(length + offSet, height, offSet),

		startCorner,
		startCorner + glm::vec3(0, 0, -width),
		startCorner + glm::vec3(-offSet, height, -width - offSet),
		startCorner + glm::vec3(-offSet, height, offSet),

		startCorner,
		startCorner + glm::vec3(length, 0,  0),
		startCorner + glm::vec3(length, 0, -width),
		startCorner + glm::vec3(0, 0, -width),

		startCorner + glm::vec3(length, 0,  0),
		startCorner + glm::vec3(length, 0, -width),
		startCorner + glm::vec3(length + offSet, height, -width - offSet),
		startCorner + glm::vec3(length + offSet, height, offSet),

		startCorner + glm::vec3(length, 0, -width),
		startCorner + glm::vec3(0, 0, -width),
		startCorner + glm::vec3(-offSet, height, -width - offSet),
		startCorner + glm::vec3(length + offSet, height, -width - offSet),
	};

	std::vector<glm::vec3> normals
	{
		glm::vec3(0, 0, 1),
		glm::vec3(0, 0, 1),
		glm::vec3(0, 0, 1),
		glm::vec3(0, 0, 1),

		glm::vec3(-1, 0, 0),
		glm::vec3(-1, 0, 0),
		glm::vec3(-1, 0, 0),
		glm::vec3(-1, 0, 0),

		glm::vec3(0, -1, 0),
		glm::vec3(0, -1, 0),
		glm::vec3(0, -1, 0),
		glm::vec3(0, -1, 0),

		glm::vec3(1, 0, 0),
		glm::vec3(1, 0, 0),
		glm::vec3(1, 0, 0),
		glm::vec3(1, 0, 0),

		glm::vec3(0, 0, -1),
		glm::vec3(0, 0, -1),
		glm::vec3(0, 0, -1),
		glm::vec3(0, 0, -1)
	};

	std::vector<glm::vec2> textureCoords
	{
		glm::vec2(0, 0),
		glm::vec2(1, 0),
		glm::vec2(0, 1),
		glm::vec2(1, 1),

		glm::vec2(1, 0),
		glm::vec2(0, 0),
		glm::vec2(0, 1),
		glm::vec2(1, 1),

		glm::vec2(0, 0),
		glm::vec2(1, 0),
		glm::vec2(1, 1),
		glm::vec2(0, 1),

		glm::vec2(0, 0),
		glm::vec2(1, 0),
		glm::vec2(1, 1),
		glm::vec2(0, 1),

		glm::vec2(0, 0),
		glm::vec2(1, 0),
		glm::vec2(1, 1),
		glm::vec2(0, 1)
	};

	std::vector<unsigned short> indices =
	{
		0, 1, 2,
		1, 3, 2,

		7, 5, 4,
		7, 6, 5,

		8, 11, 9,
		11, 10, 9,

		12, 14, 15,
		12, 13, 14,

		16, 17, 19,
		17, 18, 19
	};

	Mesh* object = new Mesh(name);
	object->InitFromData(positions, normals, textureCoords, indices);
	return object;
}


Mesh* Object3DHome::CreateCuboid(std::string name, float length, float width, float height)
{

	glm::vec3 startCorner = glm::vec3(0, 0, 0.71f);
	std::vector<glm::vec3> positions
	{
		startCorner,
		startCorner + glm::vec3(length, 0,  0),
		startCorner + glm::vec3(0, height, 0),
		startCorner + glm::vec3(length, height, 0),

		startCorner + glm::vec3(0, height, 0),
		startCorner + glm::vec3(length, height, 0),
		startCorner + glm::vec3(0, height, -width),
		startCorner + glm::vec3(length, height, -width),

		startCorner,
		startCorner + glm::vec3(0, 0, -width),
		startCorner + glm::vec3(0, height, -width),
		startCorner + glm::vec3(0, height, 0),

		startCorner,
		startCorner + glm::vec3(length, 0,  0),
		startCorner + glm::vec3(length, 0, -width),
		startCorner + glm::vec3(0, 0, -width),

		startCorner + glm::vec3(length, 0,  0),
		startCorner + glm::vec3(length, 0, -width),
		startCorner + glm::vec3(length , height, -width),
		startCorner + glm::vec3(length, height, 0),

		startCorner + glm::vec3(length, 0, -width),
		startCorner + glm::vec3(0, 0, -width),
		startCorner + glm::vec3(0, height, -width),
		startCorner + glm::vec3(length, height, -width),
	};

	std::vector<glm::vec3> normals
	{
		glm::vec3(0, 0, 1),
		glm::vec3(0, 0, 1),
		glm::vec3(0, 0, 1),
		glm::vec3(0, 0, 1),

		glm::vec3(0, 1, 0),
		glm::vec3(0, 1, 0),
		glm::vec3(0, 1, 0),
		glm::vec3(0, 1, 0),

		glm::vec3(-1, 0, 0),
		glm::vec3(-1, 0, 0),
		glm::vec3(-1, 0, 0),
		glm::vec3(-1, 0, 0),

		glm::vec3(0, -1, 0),
		glm::vec3(0, -1, 0),
		glm::vec3(0, -1, 0),
		glm::vec3(0, -1, 0),

		glm::vec3(1, 0, 0),
		glm::vec3(1, 0, 0),
		glm::vec3(1, 0, 0),
		glm::vec3(1, 0, 0),

		glm::vec3(0, 0, -1),
		glm::vec3(0, 0, -1),
		glm::vec3(0, 0, -1),
		glm::vec3(0, 0, -1)
	};

	std::vector<glm::vec2> textureCoords
	{
		glm::vec2(0, 0),
		glm::vec2(1, 0),
		glm::vec2(0, 1),
		glm::vec2(1, 1),

		glm::vec2(0, 0),
		glm::vec2(1, 0),
		glm::vec2(0, 1),
		glm::vec2(1, 1),

		glm::vec2(1, 0),
		glm::vec2(0, 0),
		glm::vec2(0, 1),
		glm::vec2(1, 1),

		glm::vec2(0, 0),
		glm::vec2(1, 0),
		glm::vec2(1, 1),
		glm::vec2(0, 1),

		glm::vec2(0, 0),
		glm::vec2(1, 0),
		glm::vec2(1, 1),
		glm::vec2(0, 1),

		glm::vec2(0, 0),
		glm::vec2(1, 0),
		glm::vec2(1, 1),
		glm::vec2(0, 1)
	};

	std::vector<unsigned short> indices =
	{
		0, 1, 2,
		1, 3, 2,

		4, 5, 7,
		4, 7, 6,

		11, 9, 8,
		11, 10, 9,

		12, 15, 13,
		15, 14, 13,

		16, 18, 19,
		16, 17, 18,

		20, 21, 23,
		21, 22, 23
	};

	Mesh* object = new Mesh(name);
	object->InitFromData(positions, normals, textureCoords, indices);
	return object;
}

Mesh* Object3DHome::CreateCeiling(std::string name, float length, float width, float height)
{

	glm::vec3 startCorner = glm::vec3(0, 0, 0.71f);
	std::vector<glm::vec3> positions
	{
		startCorner + glm::vec3(0, height, 0),
		startCorner + glm::vec3(length, height, 0),
		startCorner + glm::vec3(0, height, -width),
		startCorner + glm::vec3(length, height, -width),
	};

	std::vector<glm::vec3> normals
	{
		glm::vec3(0, -1, 0),
		glm::vec3(0, -1, 0),
		glm::vec3(0, -1, 0),
		glm::vec3(0, -1, 0),
	};

	std::vector<glm::vec2> textureCoords
	{
		glm::vec2(0, 0),
		glm::vec2(1, 0),
		glm::vec2(0, 1),
		glm::vec2(1, 1),

	};

	std::vector<unsigned short> indices =
	{
		0, 1, 3,
		0, 3, 2,
	};

	Mesh* object = new Mesh(name);
	object->InitFromData(positions, normals, textureCoords, indices);
	return object;
}

Mesh* Object3DHome::CreateFloor(std::string name, float length, float width, float height)
{

	glm::vec3 startCorner = glm::vec3(0, 0, 0.71f);
	std::vector<glm::vec3> positions
	{
		startCorner,
		startCorner + glm::vec3(length, 0,  0),
		startCorner + glm::vec3(length, 0, -width),
		startCorner + glm::vec3(0, 0, -width),
	};

	std::vector<glm::vec3> normals
	{
		glm::vec3(0, 1, 0),
		glm::vec3(0, 1, 0),
		glm::vec3(0, 1, 0),
		glm::vec3(0, 1, 0),
	};

	std::vector<glm::vec2> textureCoords
	{
		glm::vec2(0, 0),
		glm::vec2(1, 0),
		glm::vec2(1, 1),
		glm::vec2(0, 1),
	};

	std::vector<unsigned short> indices =
	{
		0, 3, 1,
		3, 2, 1,
	};

	Mesh* object = new Mesh(name);
	object->InitFromData(positions, normals, textureCoords, indices);
	return object;
}

Mesh* Object3DHome::CreateRoom(std::string name, float length, float width, float height)
{

	glm::vec3 startCorner = glm::vec3(0, 0, 0.71f);
	std::vector<glm::vec3> positions
	{
		startCorner,
		startCorner + glm::vec3(length, 0,  0),
		startCorner + glm::vec3(0, height, 0),
		startCorner + glm::vec3(length, height, 0),

		startCorner,
		startCorner + glm::vec3(0, 0, -width),
		startCorner + glm::vec3(0, height, -width),
		startCorner + glm::vec3(0, height, 0),

		startCorner + glm::vec3(length, 0,  0),
		startCorner + glm::vec3(length, 0, -width),
		startCorner + glm::vec3(length , height, -width),
		startCorner + glm::vec3(length, height, 0),

		startCorner + glm::vec3(length, 0, -width),
		startCorner + glm::vec3(0, 0, -width),
		startCorner + glm::vec3(0, height, -width),
		startCorner + glm::vec3(length, height, -width),
	};

	std::vector<glm::vec3> normals
	{
		glm::vec3(0, 0, -1),
		glm::vec3(0, 0, -1),
		glm::vec3(0, 0, -1),
		glm::vec3(0, 0, -1),

		glm::vec3(1, 0, 0),
		glm::vec3(1, 0, 0),
		glm::vec3(1, 0, 0),
		glm::vec3(1, 0, 0),

		glm::vec3(-1, 0, 0),
		glm::vec3(-1, 0, 0),
		glm::vec3(-1, 0, 0),
		glm::vec3(-1, 0, 0),

		glm::vec3(0, 0, 1),
		glm::vec3(0, 0, 1),
		glm::vec3(0, 0, 1),
		glm::vec3(0, 0, 1)
	};

	std::vector<glm::vec2> textureCoords
	{
		glm::vec2(0, 0),
		glm::vec2(1, 0),
		glm::vec2(0, 1),
		glm::vec2(1, 1),

		glm::vec2(1, 0),
		glm::vec2(0, 0),
		glm::vec2(0, 1),
		glm::vec2(1, 1),

		glm::vec2(0, 0),
		glm::vec2(1, 0),
		glm::vec2(1, 1),
		glm::vec2(0, 1),

		glm::vec2(0, 0),
		glm::vec2(1, 0),
		glm::vec2(1, 1),
		glm::vec2(0, 1)
	};

	std::vector<unsigned short> indices =
	{
		0, 1, 2,
		1, 3, 2,

		7, 5, 4,
		7, 6, 5,

		8, 10, 11,
		8, 9, 10,

		12, 13, 15,
		13, 14, 15
	};

	Mesh* object = new Mesh(name);
	object->InitFromData(positions, normals, textureCoords, indices);
	return object;
}

Mesh* Object3DHome::CreateBank(std::string name, glm::vec3 startCorner, float length, float width, float height, float offset)
{
	float offset2 = 0.02f;
	std::vector<glm::vec3> positions
	{
		startCorner,
		startCorner + glm::vec3(length, 0,  0),
		startCorner + glm::vec3(0, height, 0),
		startCorner + glm::vec3(length, height, 0),

		startCorner + glm::vec3(0, height, 0),
		startCorner + glm::vec3(length, height, 0),
		startCorner + glm::vec3(offset, height, -width),
		startCorner + glm::vec3(length - offset, height, -width),

		startCorner,
		startCorner + glm::vec3(offset, 0, -width + offset2),
		startCorner + glm::vec3(offset, height, -width),
		startCorner + glm::vec3(0, height, 0),

		startCorner,
		startCorner + glm::vec3(length, 0,  0),
		startCorner + glm::vec3(length - offset, 0, -width + offset2),
		startCorner + glm::vec3(offset, 0, -width + offset2),

		startCorner + glm::vec3(length, 0,  0),
		startCorner + glm::vec3(length - offset, 0, -width + offset2),
		startCorner + glm::vec3(length - offset , height, -width),
		startCorner + glm::vec3(length, height, 0),

		startCorner + glm::vec3(length - offset, 0, -width + offset2),
		startCorner + glm::vec3(offset, 0, -width + offset2),
		startCorner + glm::vec3(offset, height, -width),
		startCorner + glm::vec3(length - offset, height, -width),
	};

	std::vector<glm::vec3> normals
	{
		glm::vec3(0, 0, 1),
		glm::vec3(0, 0, 1),
		glm::vec3(0, 0, 1),
		glm::vec3(0, 0, 1),

		glm::vec3(0, 1, 0),
		glm::vec3(0, 1, 0),
		glm::vec3(0, 1, 0),
		glm::vec3(0, 1, 0),

		glm::vec3(-1, 0, 0),
		glm::vec3(-1, 0, 0),
		glm::vec3(-1, 0, 0),
		glm::vec3(-1, 0, 0),

		glm::vec3(0, -1, 0),
		glm::vec3(0, -1, 0),
		glm::vec3(0, -1, 0),
		glm::vec3(0, -1, 0),

		glm::vec3(1, 0, 0),
		glm::vec3(1, 0, 0),
		glm::vec3(1, 0, 0),
		glm::vec3(1, 0, 0),

		glm::vec3(0, 0, -1),
		glm::vec3(0, 0, -1),
		glm::vec3(0, 0, -1),
		glm::vec3(0, 0, -1)
	};

	std::vector<glm::vec2> textureCoords
	{
		glm::vec2(0, 0),
		glm::vec2(1, 0),
		glm::vec2(0, 1),
		glm::vec2(1, 1),

		glm::vec2(0, 0),
		glm::vec2(1, 0),
		glm::vec2(0, 1),
		glm::vec2(1, 1),

		glm::vec2(1, 0),
		glm::vec2(0, 0),
		glm::vec2(0, 1),
		glm::vec2(1, 1),

		glm::vec2(0, 0),
		glm::vec2(1, 0),
		glm::vec2(1, 1),
		glm::vec2(0, 1),

		glm::vec2(0, 0),
		glm::vec2(1, 0),
		glm::vec2(1, 1),
		glm::vec2(0, 1),

		glm::vec2(0, 0),
		glm::vec2(1, 0),
		glm::vec2(1, 1),
		glm::vec2(0, 1)
	};

	std::vector<unsigned short> indices =
	{
		0, 1, 2,
		1, 3, 2,

		4, 5, 7,
		4, 7, 6,

		11, 9, 8,
		11, 10, 9,

		12, 15, 13,
		15, 14, 13,

		16, 18, 19,
		16, 17, 18,

		20, 21, 23,
		21, 22, 23
	};

	Mesh* object = new Mesh(name);
	object->InitFromData(positions, normals, textureCoords, indices);
	return object;
}



Mesh* Object3DHome::CreateCylinder(std::string name, float radius, float length)
{
	int triangles = 1000;
	double k = 0;
	glm::vec3 center = glm::vec3(0, 0, 0.71f);
	float circleX = center.x;
	float circleZ = center.z;
	float circumference = (float)(2 * M_PI);

	Mesh* cylinder = new Mesh(name);
	std::vector<glm::vec3> positions;
	std::vector<glm::vec3> normals;
	std::vector<glm::vec2> textureCoords;
	std::vector<unsigned short> indices;

	

	for (double i = 0; i < triangles; i++) {
		positions.push_back(glm::vec3(circleX, length, circleZ));
		positions.push_back(glm::vec3(circleX + (radius * cos(i * circumference / triangles)), length, circleZ + (radius * sin(i * circumference / triangles))));
		positions.push_back(glm::vec3(circleX + (radius * cos((i+1) * circumference / triangles)), length, circleZ + (radius * sin((i + 1) * circumference / triangles))));
		positions.push_back(glm::vec3(circleX + (radius * cos((i + 1) * circumference / triangles)), -length, circleZ + (radius * sin((i + 1) * circumference / triangles))));
		positions.push_back(glm::vec3(circleX + (radius * cos(i * circumference / triangles)), -length, circleZ + (radius * sin(i * circumference / triangles))));
		positions.push_back(glm::vec3(circleX, -length, circleZ));
		normals.push_back(glm::vec3(0, -1, 0));
		textureCoords.push_back(glm::vec2(k, k));
		k += 0.02;
	}

	for (int i = 0; i < 6 * triangles; i++) {
		indices.push_back(i);
	}

	cylinder->SetDrawMode(GL_TRIANGLE_STRIP);
	cylinder->InitFromData(positions, normals, textureCoords, indices);

	return cylinder;
}
