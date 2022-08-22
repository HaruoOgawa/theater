#include "LTreeModel.h"
#include "LTree.h"
#include "GraphicsEngine/Component/MeshRendererComponent.h"
#include "GraphicsEngine/Component/TransformComponent.h"
#include "GraphicsEngine/Graphics/ShaderLib.h"

namespace myapp {
	LTreeModel::LTreeModel() :
		m_TreeRenderer(nullptr)
	{
		Start();
	}

	void LTreeModel::Start()
	{
		// –Ø‚Ìƒ‚ƒfƒ‹‚ğ¶¬
		{
			//
			std::vector<std::vector<float>> VertexData; std::vector<int> Dimention; std::vector<unsigned short> Indices;
			LTree Tree;
			Tree.Generate(VertexData, Dimention, Indices);

			//
			m_TreeRenderer = std::make_shared<MeshRendererComponent>(
				std::make_shared<TransformComponent>(),
				RenderingSurfaceType::RASTERIZER,
				VertexData, Dimention, Indices,
				std::string(
					#include "../../Shader/Forest/LTree.vert"
				),
				shaderlib::ShaderLib::Standard_frag,
				std::string(
					#include "../../Shader/Forest/LTree.geom"
				)
			);
		}
	}

	void LTreeModel::Update()
	{
	}

	void LTreeModel::Draw()
	{
		m_TreeRenderer->Draw(GL_LINES, true, 512, [this]() {
			m_TreeRenderer->m_material->SetFloatUniform("_TreeMaxRadius", 0.05f);
			m_TreeRenderer->m_material->SetIntUniform("_TreeSegment", 3);
		});
	}
}