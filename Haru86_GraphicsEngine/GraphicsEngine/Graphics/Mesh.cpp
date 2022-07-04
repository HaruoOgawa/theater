#include "Mesh.h"
#include "./Primitive.h"
#include <vector>

Mesh::Mesh(PrimitiveType primType) {
	CreateMesh(primType);
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
	case PrimitiveType::POINT:
		Primitive::CreatePoint(&vertices, &dimention, &indices);
		break;
	case PrimitiveType::SPHERE:
		Primitive::CreateSphere(&vertices, &dimention, &indices);
		break;
	default:
		Primitive::CreateBoard(&vertices, &dimention, &indices);
		break;
	}
	
	std::shared_ptr<Primitive> prim = std::make_shared<Primitive>(vertices, dimention, indices);
	m_primitives.push_back(prim);

}

void Mesh::Draw(GLenum DrawVertexWay) {
	for (int i = 0; i < m_primitives.size();i++) {
		m_primitives[i]->Draw(DrawVertexWay);
	}
}

void Mesh::DrawInstancedWithMesh(int count, GLenum rendermode) {
	for (int i = 0; i < m_primitives.size(); i++) {
		m_primitives[i]->SetActive();
		glDrawElementsInstanced(rendermode, m_primitives[i]->mNumIndices, GL_UNSIGNED_SHORT, nullptr, count);
	}
}