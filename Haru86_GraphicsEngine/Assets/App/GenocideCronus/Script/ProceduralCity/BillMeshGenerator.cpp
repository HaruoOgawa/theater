#include "BillMeshGenerator.h"
#include "GraphicsEngine/Graphics/Mesh.h"
#include "GraphicsEngine/Component/TransformComponent.h"

namespace myapp {
	void BillMeshGenerator::Generate(std::vector<std::vector<float>>& VertexData, std::vector<int>& Dimention, std::vector<unsigned short>& Indices)
	{
		// 初期化
		unsigned int ATTRIBUTENUM = 2;
		VertexData.resize(ATTRIBUTENUM);

		{
			// Debug
			std::shared_ptr<TransformComponent> TRS=std::make_shared<TransformComponent>();
			TRS->CalMatrix();
			PrepareBoxVertexData(VertexData, Indices, TRS->m_mMatrix);
		}
		
		{
			// Debug2
			std::shared_ptr<TransformComponent> TRS=std::make_shared<TransformComponent>();
			TRS->m_position = glm::vec3(0.0f, 2.0f, 0.0f);
			
			TRS->CalMatrix();
			PrepareBoxVertexData(VertexData, Indices, TRS->m_mMatrix);
		}
		
		{
			// Debug3
			std::shared_ptr<TransformComponent> TRS=std::make_shared<TransformComponent>();
			TRS->m_position = glm::vec3(0.0f, 4.0f, 0.0f);
			TRS->m_scale = glm::vec3(4.0f, 1.0f, 4.0f);

			TRS->CalMatrix();
			PrepareBoxVertexData(VertexData, Indices, TRS->m_mMatrix);
		}

		// データをまとめる
		Dimention.push_back(3);
		Dimention.push_back(3);
	}

	void BillMeshGenerator::PrepareBoxVertexData(std::vector<std::vector<float>>& VertexData, std::vector<unsigned short>& Indices, glm::mat4 LocalTransMatrix)
	{
		// 頂点データは初期化されている前提である
		if (VertexData.size() <= 0)return;

		// データの準備
		std::vector<float> vertex = { // VertexData Index: 0
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

		std::vector<float> normal = { // VertexData Index: 1
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

		unsigned int IndexOffset = (Indices.size() <= 0) ? 0 : (Indices[Indices.size() - 1]);
		std::vector<unsigned int> index = {
			//0 +Z
			0 + IndexOffset,1 + IndexOffset,2 + IndexOffset,
			2 + IndexOffset,3 + IndexOffset,0 + IndexOffset,
			//1 -Z
			4 + IndexOffset,5 + IndexOffset,6 + IndexOffset,
			6 + IndexOffset,7 + IndexOffset,4 + IndexOffset,
			//2 -X
			8 + IndexOffset,9 + IndexOffset,10 + IndexOffset,
			10 + IndexOffset,11 + IndexOffset,8 + IndexOffset,
			//3 +X
			12 + IndexOffset,13 + IndexOffset,14 + IndexOffset,
			14 + IndexOffset,15 + IndexOffset,12 + IndexOffset,
			//4 -Y
			16 + IndexOffset,17 + IndexOffset,18 + IndexOffset,
			18 + IndexOffset,19 + IndexOffset,16 + IndexOffset,
			//5 +Y
			20 + IndexOffset,21 + IndexOffset,22 + IndexOffset,
			22 + IndexOffset,23 + IndexOffset,20 + IndexOffset,
		};

		// 頂点データを構築
		for (int i = 0; i < vertex.size(); i += 3)
		{
			//
			glm::vec4 v = glm::vec4(vertex[i], vertex[i + 1], vertex[i + 2],1.0f);
			glm::vec4 n = glm::vec4(normal[i], normal[i + 1], normal[i + 2],0.0f);

			// 行列をかける
			v = LocalTransMatrix * v;
			n = LocalTransMatrix * n;

			// 底が原点になるように調整
			v.y += 0.5f;

			// Vetices
			VertexData[0].push_back(v.x);
			VertexData[0].push_back(v.y);
			VertexData[0].push_back(v.z);

			// Normal
			VertexData[1].push_back(n.x);
			VertexData[1].push_back(n.y);
			VertexData[1].push_back(n.z);

			// IsWindow
			//VertexData[2]
		}
		
		// インデックスデータ
		for (const auto& Val : index)
		{
			Indices.push_back(Val);
		}
	}
}