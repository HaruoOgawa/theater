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

void Primitive::Draw(GLenum DrawVertexWay) {
	SetActive();
	glDrawElements(DrawVertexWay, mNumIndices, GL_UNSIGNED_SHORT, nullptr);
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

void Primitive::CreateSphere(std::vector<std::vector<float>>* vertices, std::vector<int>* dimention, std::vector<unsigned short>* indices, float row, float column, float rad) {
	float pi = 3.14159265f;

	std::vector<float> vertex;
	std::vector<float> normal;
	std::vector<float> texcoord;

	for (auto i = 0; i <= row; i++) {
		auto r = pi / row * i;
		auto ry = glm::cos(r);
		auto rr = glm::sin(r);
		for (auto ii = 0; ii <= column; ii++) {
			float tr = pi * 2 / column * ii;
			float tx = rr * rad * glm::cos(tr);
			float ty = ry * rad;
			float tz = rr * rad * glm::sin(tr);
			float rx = rr * glm::cos(tr);
			float rz = rr * glm::sin(tr);

			vertex.push_back(tx);
			vertex.push_back(ty);
			vertex.push_back(tz);
			normal.push_back(rx);
			normal.push_back(ry);
			normal.push_back(rz);
			texcoord.push_back(static_cast<float>(1 - 1 / column * ii));
			texcoord.push_back(static_cast<float>(1 / row * i));
		}
	}

	vertices->push_back(vertex);
	vertices->push_back(normal);
	vertices->push_back(texcoord);

	dimention->push_back(3);
	dimention->push_back(3);
	dimention->push_back(2);

	for (int i = 0; i < row; i++) {
		for (int ii = 0; ii < column; ii++) {
			int r = (column + 1) * i + ii;

			indices->emplace_back(static_cast<int>(r));
			indices->emplace_back(static_cast<int>(r + 1));
			indices->emplace_back(static_cast<int>(r + column + 2));
			indices->emplace_back(static_cast<int>(r));
			indices->emplace_back(static_cast<int>(r + column + 2));
			indices->emplace_back(static_cast<int>(r + column + 1));
		}
	}
}
