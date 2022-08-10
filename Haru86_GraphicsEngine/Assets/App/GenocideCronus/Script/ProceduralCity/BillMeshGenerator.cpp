#include "BillMeshGenerator.h"
#include "GraphicsEngine/Graphics/Mesh.h"
#include "GraphicsEngine/Component/TransformComponent.h"

namespace myapp {
	void BillMeshGenerator::Generate(std::vector<std::vector<float>>& VertexData, std::vector<int>& Dimention, std::vector<unsigned short>& Indices)
	{
		// 初期化
		m_LastIndex = 0;
		unsigned int ATTRIBUTENUM = 3;
		VertexData.resize(ATTRIBUTENUM);

		// 基本パラメーター
		float AdjustVal = 0.1f;
		float BillRadius = 5.0f* AdjustVal;
		float BillHeight = 20.0f* AdjustVal;
		int HolDelimiterNum = 10;
		int VerDelimiterNum = 10;
		float HolResizeRate = 1.05f;
		float VerResizeRate = 1.1f;
		float HolDelimiterThickness = 0.25f* AdjustVal;
		float VerDelimiterThickness = 0.1f * AdjustVal;

		// Base(窓となる部分)
		{
			std::shared_ptr<TransformComponent> TRS = std::make_shared<TransformComponent>();
			TRS->m_scale = glm::vec3(BillRadius, BillHeight*1.1f, BillRadius);

			TRS->CalMatrix();
			PrepareBoxVertexData(VertexData, Indices, TRS->m_mMatrix,false,false,0,false);
		}

		// Y方向の区切り
		{
			for (int n = 0; n < HolDelimiterNum; n++)
			{
				std::shared_ptr<TransformComponent> TRS = std::make_shared<TransformComponent>();
				TRS->m_scale = glm::vec3(BillRadius * HolResizeRate, HolDelimiterThickness, BillRadius * HolResizeRate);
				
				float YOffset = ( BillHeight / static_cast<float>(HolDelimiterNum) ) * static_cast<float>(n+1);
				TRS->m_position = glm::vec3(0.0f, YOffset, 0.0f);

				TRS->CalMatrix();
				PrepareBoxVertexData(VertexData, Indices, TRS->m_mMatrix,false,false, static_cast<float>(n),false);
			}
		}
		
		// X方向の区切り
		{
			int HalfNum = static_cast<int>(VerDelimiterNum / 2);
			for (int n = - HalfNum; n < HalfNum + 1; n++)
			{
				std::shared_ptr<TransformComponent> TRS = std::make_shared<TransformComponent>();
				TRS->m_scale = glm::vec3(VerDelimiterThickness, BillHeight * VerResizeRate, BillRadius * VerResizeRate);
				
				float XOffset = 2.0f*( BillRadius / static_cast<float>(VerDelimiterNum) ) * static_cast<float>(n);
				TRS->m_position = glm::vec3(XOffset, 0.0f, 0.0f);

				TRS->CalMatrix();

				// ループの最初と最後を窓とする
				bool IsWindow = (n == -HalfNum || n == HalfNum);

				//
				PrepareBoxVertexData(VertexData, Indices, TRS->m_mMatrix, IsWindow,true,static_cast<float>(n),true);
			}
		}

		// Z方向の区切り
		{
			int HalfNum = static_cast<int>(VerDelimiterNum / 2);
			for (int n = -HalfNum; n < HalfNum + 1; n++)
			{
				std::shared_ptr<TransformComponent> TRS = std::make_shared<TransformComponent>();
				TRS->m_scale = glm::vec3(BillRadius * VerResizeRate, BillHeight * VerResizeRate, VerDelimiterThickness);

				float ZOffset = 2.0f * (BillRadius / static_cast<float>(VerDelimiterNum)) * static_cast<float>(n);
				TRS->m_position = glm::vec3(0.0f, 0.0f, ZOffset);

				TRS->CalMatrix();

				// ループの最初と最後を窓とする
				bool IsWindow = (n == -HalfNum || n == HalfNum);

				//
				PrepareBoxVertexData(VertexData, Indices, TRS->m_mMatrix, IsWindow,true, static_cast<float>(n),false);
			}
		}

		// ビルの4隅に柱的なのを立てる(窓が切れてて違和感があるから)
		{
			for (float x = -1.0f; x<=1.0f; x++)
			{
				for (float z = -1.0f; z <= 1.0f; z++)
				{
					//
					if (x == 0.0f || z == 0.0f)continue;

					//
					std::shared_ptr<TransformComponent> TRS = std::make_shared<TransformComponent>();
					TRS->m_scale = glm::vec3(BillRadius * 0.1f,BillHeight * 1.1f, BillRadius * 0.1f);
					float AdjustPos = 1.05f;
					TRS->m_position = glm::vec3(BillRadius  * x * AdjustPos, 0.0f, BillRadius  * z * AdjustPos);

					TRS->CalMatrix();
					PrepareBoxVertexData(VertexData, Indices, TRS->m_mMatrix,false,false,0,false);
				}
			}
		}

		// データをまとめる
		Dimention.push_back(3);
		Dimention.push_back(3);
		Dimention.push_back(4);
	}

	void BillMeshGenerator::PrepareBoxVertexData(std::vector<std::vector<float>>& VertexData, std::vector<unsigned short>& Indices, glm::mat4 LocalTransMatrix,
		bool IsWindow, bool IsVertical, float order, bool IsXAxis)
	{
		// 頂点データは初期化されている前提である
		if (VertexData.size() <= 0)return;

		// データの準備
		std::vector<float> vertex = { // VertexData Index: 0
			//0 +Z
			-1.0f,-1.0f,1.0f,
			-1.0f,1.0f,1.0f,
			1.0f,1.0f,1.0f,
			1.0f,-1.0f,1.0f,
			//1 -Z
			-1.0f,-1.0f,-1.0f,
			-1.0f,1.0f,-1.0f,
			1.0f,1.0f,-1.0f,
			1.0f,-1.0f,-1.0f,
			//2 -X
			-1.0f,-1.0f,1.0f,
			-1.0f,1.0f,1.0f,
			-1.0f,1.0f,-1.0f,
			-1.0f,-1.0f,-1.0f,
			//3 +X
			1.0f,-1.0f,1.0f,
			1.0f,1.0f,1.0f,
			1.0f,1.0f,-1.0f,
			1.0f,-1.0f,-1.0f,
			//4  -Y
			-1.0f,-1.0f,1.0f,
			-1.0f,-1.0f,-1.0f,
			1.0f,-1.0f,-1.0f,
			1.0f,-1.0f,1.0f,
			//5 +Y
			-1.0f,1.0f,1.0f,
			-1.0f,1.0f,-1.0f,
			1.0f,1.0f,-1.0f,
			1.0f,1.0f,1.0f,
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

		//unsigned int m_LastIndex = (Indices.size() <= 0) ? 0 : (Indices[Indices.size() - 1]);
		std::vector<unsigned int> index = {
			//0 +Z
			0 + m_LastIndex,1 + m_LastIndex,2 + m_LastIndex,
			2 + m_LastIndex,3 + m_LastIndex,0 + m_LastIndex,
			//1 -Z
			4 + m_LastIndex,5 + m_LastIndex,6 + m_LastIndex,
			6 + m_LastIndex,7 + m_LastIndex,4 + m_LastIndex,
			//2 -X
			8 + m_LastIndex,9 + m_LastIndex,10 + m_LastIndex,
			10 + m_LastIndex,11 + m_LastIndex,8 + m_LastIndex,
			//3 +X
			12 + m_LastIndex,13 + m_LastIndex,14 + m_LastIndex,
			14 + m_LastIndex,15 + m_LastIndex,12 + m_LastIndex,
			//4 -Y
			16 + m_LastIndex,17 + m_LastIndex,18 + m_LastIndex,
			18 + m_LastIndex,19 + m_LastIndex,16 + m_LastIndex,
			//5 +Y
			20 + m_LastIndex,21 + m_LastIndex,22 + m_LastIndex,
			22 + m_LastIndex,23 + m_LastIndex,20 + m_LastIndex,
		};

		// 頂点データを構築
		for (int i = 0; i < vertex.size(); i += 3)
		{
			//
			glm::vec4 v = glm::vec4(vertex[i], vertex[i + 1], vertex[i + 2],1.0f);
			glm::vec4 n = glm::vec4(normal[i], normal[i + 1], normal[i + 2],0.0f);

			//// 底が原点になるように調整
			//v.y += 1.0f;

			// 行列をかける
			v = LocalTransMatrix * v;
			n = LocalTransMatrix * n;

			// 底が原点になるように調整
			v.y += glm::abs(v.y)*0.5f;

			// Vetices
			VertexData[0].push_back(v.x);
			VertexData[0].push_back(v.y);
			VertexData[0].push_back(v.z);

			// Normal
			VertexData[1].push_back(n.x);
			VertexData[1].push_back(n.y);
			VertexData[1].push_back(n.z);

			// BillInfo
			// IsWindow
			VertexData[2].push_back( (IsWindow) ? 1.0f : 0.0f );
			VertexData[2].push_back( (IsVertical) ? 1.0f : 0.0f );
			VertexData[2].push_back(order);
			VertexData[2].push_back( (IsXAxis) ? 1.0f : 0.0f );
		}
		
		// インデックスデータ
		for (const auto& Val : index)
		{
			//
			Indices.push_back(Val);

			// 最後のインデックスを保持
			m_LastIndex = glm::max(m_LastIndex, Val + 1);
		}
	}
}