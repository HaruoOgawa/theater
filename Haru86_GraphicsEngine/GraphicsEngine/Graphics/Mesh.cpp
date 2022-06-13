#include "Mesh.h"
#include <glew.h>
#include "./Primitive.h"
#include <vector>

//default Mesh
Mesh::Mesh() {
	localMatrix = glm::mat4(1.0);

}

Mesh::Mesh(PrimitiveType primType) {
	CreateMesh(primType);
}

Mesh::~Mesh() {
	UnLoadData();
}

void Mesh::CreateMesh(PrimitiveType primType) {
	
	std::vector<std::vector<float>> vertices;
	std::vector<int> dimention;
	std::vector<unsigned short> indices;

	switch (primType)
	{
	case PrimitiveType::BOARD:
		Primitive::CreateBoard(&vertices, &dimention,&indices);
		break;
	case PrimitiveType::CUBE:
		Primitive::CreateCube(&vertices, &dimention, &indices);
		break;
	case PrimitiveType::SPHERE:
		Primitive::CreateSphere(&vertices, &dimention, &indices);
		break;
	case PrimitiveType::POINT:
		Primitive::CreatePoint(&vertices, &dimention, &indices);
		break;
	default:
		Primitive::CreateBoard(&vertices, &dimention, &indices);
		break;
	}
	
	std::shared_ptr<Primitive> prim = std::make_shared<Primitive>(vertices, dimention, indices);
	m_primitives.push_back(prim);

}

void Mesh::Draw() {
	for (int i = 0; i < m_primitives.size();i++) {
		m_primitives[i]->Draw();
	}

}

void Mesh::Draw(GLDrawType drawType) {
	for (int i = 0; i < m_primitives.size();i++) {
		m_primitives[i]->Draw("tessellation");
	}

}

const std::vector<std::shared_ptr<Primitive>>& Mesh::GetPrimitiveList()const {
	return m_primitives;
}

void Mesh::SetPrimitive(const std::shared_ptr<Primitive>& Primitive) {
	m_primitives.push_back(Primitive);
}

void Mesh::UnLoadData() {
	
}