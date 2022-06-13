#include "Primitive.h"
#include <glm/glm.hpp>
#include <glew.h>
#include "GraphicsEngine/Graphics/Mesh.h"

Primitive::Primitive(const std::vector<float>& vertices, const std::vector<int>& indices)
{
	CreatePrimitive(vertices, indices);
}

Primitive::Primitive(const std::vector<std::vector<float>>& vertices, const std::vector<int>& dimention, const std::vector<unsigned short>& indices):
	m_vertices(vertices),
	m_dimention(dimention),
	m_indices(indices)
{
	CreatePrimitive(vertices,dimention, indices);
}
Primitive::~Primitive() {
	UnLoadData();
}

void Primitive::Release() {
	m_vertices.clear();
	m_dimention.clear();
	m_indices.clear();
}

void Primitive::CreatePrimitive(const std::vector<float>& vertices, const std::vector<int>& indices) {
	unsigned int mVertexBuffer;
	unsigned int mIndexBuffer;
	
	unsigned int mNumVerts = vertices.size() / 5;
	mNumIndices = indices.size();

	glGenVertexArrays(1, &mVertexArray);
	glBindVertexArray(mVertexArray);

	glGenBuffers(1, &mVertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, mVertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, mNumVerts * 8 * sizeof(float), vertices.data(), GL_STATIC_DRAW);

	glGenBuffers(1, &mIndexBuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIndexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, mNumIndices * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), 0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float),
		reinterpret_cast<void*>(sizeof(float) * 3)
	);
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float),
		reinterpret_cast<void*>(sizeof(float) * 6)
	);

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
	glDrawElements(GL_TRIANGLES, GetNumIndices(), GL_UNSIGNED_SHORT, nullptr);
}

void Primitive::Draw(std::string drawType) {
	SetActive();
	glDrawElements(GL_PATCHES, GetNumIndices(), GL_UNSIGNED_SHORT, nullptr);
}

const std::vector<std::vector<float>>& Primitive::GetVertices()const {
	return m_vertices;
}

const std::vector<int>& Primitive::GetDimentions()const {
	return m_dimention;
}

const std::vector<unsigned short>& Primitive::GetIndices()const {
	return m_indices;
}

void Primitive::SetActive() {
	glBindVertexArray(mVertexArray);
}

void Primitive::UnLoadData() {
	Release();
	glDeleteVertexArrays(1, &mVertexArray);
}

void Primitive::CreateBoard(std::vector<float>* vertices, std::vector<int>* indices) {
	std::vector<float> v= {
		-0.5f, 0.5f, 0.f, 0.f, 0.f, 0.0f, 0.f, 1.f, // top left
		0.5f, 0.5f, 0.f, 0.f, 0.f, 0.0f, 1.f, 1.f, // top right
		0.5f,-0.5f, 0.f, 0.f, 0.f, 0.0f, 1.f, 0.f, // bottom right
		-0.5f,-0.5f, 0.f, 0.f, 0.f, 0.0f, 0.f, 0.f  // bottom left
	};
	std::vector<int> i= {
		0, 1, 2,
		2, 3, 0
	};

	for (auto data : v) {
		vertices->emplace_back(data);
	}

	for (auto data : i) {
		indices->emplace_back(data);
	}
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

void Primitive::CreateCube(std::vector<float>* vertices, std::vector<int>* indices) {
	std::vector<float> v = {
		-0.5f,-0.5f,-0.5f,0.0f,0.0f,-1.0f,0.0f,0.0f,
		-0.5f,0.5f,-0.5f,0.0f,0.0f,-1.0f,0.0f,1.0f,
		0.5f,0.5f,-0.5f,0.0f,0.0f,-1.0f,1.0f,1.0f,
		0.5f,-0.5f,-0.5f,0.0f,0.0f,-1.0f,1.0f,0.0f,
		-0.5f,-0.5f,0.5f,0.0f,0.0f,1.0f,0.0f,0.0f,
		-0.5f,0.5f,0.5f,0.0f,0.0f,1.0f,0.0f,1.0f,
		0.5f,0.5f,0.5f,0.0f,0.0f,1.0f,1.0f,1.0f,
		0.5f,-0.5f,0.5f,0.0f,0.0f,1.0f,1.0f,0.0f,
	};

	std::vector<int> i{
		0,1,2,
		2,3,0,
		4,5,6,
		6,7,4,
		1,5,6,
		6,2,1,
		0,4,7,
		7,3,0,
		3,2,6,
		6,7,3,
		0,1,5,
		5,4,0
	};

	for(auto data : v) {
		vertices->emplace_back(data);
	}

	for(auto data : i) {
		indices->emplace_back(data);
	}
}

void Primitive::CreateCube(std::vector<std::vector<float>>* vertices, std::vector<int>* dimention, std::vector<unsigned short>* indices) {
	
	std::vector<float> vertex = {
		//0 +Z
		-0.5f,-0.5f,0.5f,
		-0.5f,0.5f,0.5f,
		0.5f,0.5f,0.5f,
		0.5f,-0.5f,0.5f,
		//1 -Z
		-0.5f,-0.5f,-0.5f,
		-0.5f,0.5f,-0.5f,
		0.5f,0.5f,-0.5f,
		0.5f,-0.5f,-0.5f,
		//2 -X
		-0.5f,-0.5f,0.5f,
		-0.5f,0.5f,0.5f,
		-0.5f,0.5f,-0.5f,
		-0.5f,-0.5f,-0.5f,
		//3 +X
		0.5f,-0.5f,0.5f,
		0.5f,0.5f,0.5f,
		0.5f,0.5f,-0.5f,
		0.5f,-0.5f,-0.5f,
		//4  -Y
		-0.5f,-0.5f,0.5f,
		-0.5f,-0.5f,-0.5f,
		0.5f,-0.5f,-0.5f,
		0.5f,-0.5f,0.5f,
		//5 +Y
		-0.5f,0.5f,0.5f,
		-0.5f,0.5f,-0.5f,
		0.5f,0.5f,-0.5f,
		0.5f,0.5f,0.5f,
	};

	std::vector<float> normal = {
		//0 +Z
		0.0f,0.0f,1.0f,
		0.0f,0.0f,1.0f,
		0.0f,0.0f,1.0f,
		0.0f,0.0f,1.0f,
		//1 -Z
		0.0f,0.0f,-1.0f,
		0.0f,0.0f,-1.0f,
		0.0f,0.0f,-1.0f,
		0.0f,0.0f,-1.0f,
		//2 -X
		-1.0f,0.0f,0.0f,
		-1.0f,0.0f,0.0f,
		-1.0f,0.0f,0.0f,
		-1.0f,0.0f,0.0f,
		//3 +X
		1.0f,0.0f,0.0f,
		1.0f,0.0f,0.0f,
		1.0f,0.0f,0.0f,
		1.0f,0.0f,0.0f,
		//4 -Y
		0.0f,-1.0f,0.0f,
		0.0f,-1.0f,0.0f,
		0.0f,-1.0f,0.0f,
		0.0f,-1.0f,0.0f,
		//5 +Y
		0.0f,1.0f,0.0f,
		0.0f,1.0f,0.0f,
		0.0f,1.0f,0.0f,
		0.0f,1.0f,0.0f,
	};

	std::vector<float> texcoord = {
		//0 +Z
		0.0f,0.0f,
		0.0f,1.0f,
		1.0f,1.0f,
		1.0f,0.0f,
		//1 -Z
		0.0f,0.0f,
		0.0f,1.0f,
		1.0f,1.0f,
		1.0f,0.0f,
		//2 -X
		0.0f,0.0f,
		0.0f,1.0f,
		1.0f,1.0f,
		1.0f,0.0f,
		//3 +X
		0.0f,0.0f,
		0.0f,1.0f,
		1.0f,1.0f,
		1.0f,0.0f,
		//4 -Y
		0.0f,0.0f,
		0.0f,1.0f,
		1.0f,1.0f,
		1.0f,0.0f,
		//5 +Y
		0.0f,0.0f,
		0.0f,1.0f,
		1.0f,1.0f,
		1.0f,0.0f,
	};

	std::vector<unsigned int> i = {
		//0 +Z
		0,1,2,
		2,3,0,
		//1 -Z
		4,5,6,
		6,7,4,
		//2 -X
		8,9,10,
		10,11,8,
		//3 +X
		12,13,14,
		14,15,12,
		//4 -Y
		16,17,18,
		18,19,16,
		//5 +Y
		20,21,22,
		22,23,20,
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

void Primitive::CreateSphere(std::vector<float>* vertices, std::vector<int>* indices, float row, float column, float rad ) {
	float pi = 3.14159265f;
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
		
			vertices->emplace_back(tx);
			vertices->emplace_back(ty);
			vertices->emplace_back(tz);
			vertices->emplace_back(rx);
			vertices->emplace_back(ry);
			vertices->emplace_back(rz);
			vertices->emplace_back(static_cast<float>(1 - 1 / column * ii));
			vertices->emplace_back(static_cast<float>(1 / row * i));
		}
	}
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
void Primitive::CreateSphere(std::vector<std::vector<float>>* vertices, std::vector<int>* dimention, std::vector<unsigned short>* indices, float row, float column, float rad ) {
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
