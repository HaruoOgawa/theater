#include "Primitive.h"
#include <glm/glm.hpp>
#include <glew.h>
#include "GraphicsEngine/Graphics/Mesh.h"

Primitive::Primitive(const std::vector<std::vector<float>>& vertices, const std::vector<int>& dimention, const std::vector<unsigned short>& indices):
	m_vertices(vertices),
	m_dimention(dimention),
	m_indices(indices)
{
	CreatePrimitive(vertices,dimention, indices);
}
Primitive::~Primitive() {
	glDeleteVertexArrays(1, &mVertexArray);
}

void Primitive::CreatePrimitive(const std::vector<std::vector<float>>& vertices, const std::vector<int>& dimention, const std::vector<unsigned short>& indices) {
	
	unsigned int mVertexBuffer;
	unsigned int mIndexBuffer;
	mNumIndices = indices.size();
	vertNum = vertices[0].size();

	glGenVertexArrays(1, &mVertexArray);
	glBindVertexArray(mVertexArray);

	
	for (int i=0;i<vertices.size();i++)
	{
		auto vert = vertices[i];
		auto dim = dimention[i];

		glGenBuffers(1, &mVertexBuffer);
		glBindBuffer(GL_ARRAY_BUFFER, mVertexBuffer);
		glBufferData(GL_ARRAY_BUFFER, vert.size() * sizeof(float), vert.data(), GL_STATIC_DRAW);

		glEnableVertexAttribArray(i);
		glVertexAttribPointer(i, dim, GL_FLOAT, GL_FALSE, dim * sizeof(float), 0);

		
	}

	glGenBuffers(1, &mIndexBuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIndexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, mNumIndices * sizeof(unsigned short), indices.data(), GL_STATIC_DRAW);

}

void Primitive::Draw() {
	SetActive();
	glDrawElements(GL_TRIANGLES, mNumIndices, GL_UNSIGNED_SHORT, nullptr);
}

void Primitive::SetActive() {
	glBindVertexArray(mVertexArray);
}


void Primitive::CreateBoard(std::vector<std::vector<float>>* vertices, std::vector<int>* dimention, std::vector<unsigned short>* indices) {
	std::vector<float> vertex= {
		-0.5f, 0.5f, 0.f,  // top left
		0.5f, 0.5f, 0.f, // top right
		0.5f,-0.5f, 0.f,// bottom right
		-0.5f,-0.5f, 0.f,    // bottom left
	};

	std::vector<float> normal = {
		0.f, 0.f, 1.0f,
		0.f, 0.f, 1.0f,
		0.f, 0.f, 1.0f,
		0.f, 0.f, 1.0f,
	};
	
	std::vector<float> texcoord = {
		 0.f, 1.f,
		 1.f, 1.f,
		 1.f, 0.f,
		 0.f, 0.f
	};

	std::vector<unsigned int> i= {
		0, 1, 2,
		2, 3, 0
	};

	vertices->push_back(vertex);
	vertices->push_back(normal);
	vertices->push_back(texcoord);

	dimention->push_back(3);
	dimention->push_back(3);
	dimention->push_back(2);

	for (auto data : i) {
		indices->emplace_back(data);
	}
}

void Primitive::CreatePoint(std::vector<std::vector<float>>* vertices, std::vector<int>* dimention, std::vector<unsigned short>* indices) {
	std::vector<float> vertex = {
		0.0f, 0.0f, 0.0f
	};

	std::vector<float> normal = {
		0.f, 0.f, 0.0f
	};

	std::vector<float> texcoord = {
		 0.0f,0.0f
	};

	std::vector<unsigned int> i = {
		0
	};

	vertices->push_back(vertex);
	vertices->push_back(normal);
	vertices->push_back(texcoord);

	dimention->push_back(3);
	dimention->push_back(3);
	dimention->push_back(2);

	for (auto data : i) {
		indices->emplace_back(data);
	}
}
