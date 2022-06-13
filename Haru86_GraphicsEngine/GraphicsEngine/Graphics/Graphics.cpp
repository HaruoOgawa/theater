#include "Graphics.h"
#include "./Mesh.h"
#include <iostream>
#include "./ComputeShader.h"
#include "GraphicsEngine/Graphics/Primitive.h"
#include <glew.h>
#include "GraphicsEngine/Graphics/Material.h"

void Graphics::DrawInstancedWithMesh(std::shared_ptr<Mesh> mesh, int count, std::shared_ptr<Material> material, GLenum rendermode) {
	for (int i = 0; i < mesh->m_primitives.size();i++) {
		mesh->m_primitives[i]->SetActive();
		glDrawElementsInstanced(rendermode, mesh->m_primitives[i]->GetNumIndices(), GL_UNSIGNED_SHORT, nullptr, count);
	}
}
