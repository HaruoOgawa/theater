#include "BillMeshGenerator.h"
#include "GraphicsEngine/Graphics/Mesh.h"

namespace myapp {
	std::shared_ptr<Mesh> BillMeshGenerator::Generate()
	{
		std::shared_ptr<Mesh> BillMesh = nullptr;

		return BillMesh;
	}

	void BillMeshGenerator::PrepareBoxVertexData(std::vector<std::vector<float>>& VertexData, std::vector<unsigned short>& Indices, glm::mat4 LocalTransMatrix, bool BottomIsOrigin)
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

		std::vector<unsigned int> index = {
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

		// 頂点データを構築
		for (int i = 0; i < vertex.size(); i += 3)
		{
			//
			glm::vec4 v = glm::vec4(vertex[i], vertex[i + 1], vertex[i + 2],1.0f);
			glm::vec4 n = glm::vec4(normal[i], normal[i + 1], normal[i + 2],1.0f);

			// 底が原点になるように調整
			v.y += 0.5f;

			// 行列をかける
			v = LocalTransMatrix * v;
			n = LocalTransMatrix * n;

			// Vetices
			VertexData[0].push_back(v.x);
			VertexData[0].push_back(v.y);
			VertexData[0].push_back(v.y);

			// Normal
			VertexData[1].push_back(n.x);
			VertexData[1].push_back(n.y);
			VertexData[1].push_back(n.y);

			// IsWindow
			//VertexData[2]
		}
		
		// インデックスデータ
		Indices.resize(Indices.size() + index.size());
		std::memcpy(&Indices[Indices.size() - index.size()], &index[0], index.size());
	}
}