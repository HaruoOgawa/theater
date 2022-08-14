#pragma once
#include <vector>
#include <memory>
#include <glm/glm.hpp>

class Mesh;

namespace myapp {
	class BillMeshGenerator
	{
		unsigned int m_LastIndex;
	private:
		void PrepareBoxVertexData(std::vector<std::vector<float>>& VertexData, std::vector<unsigned short>& Indices, glm::mat4 LocalTransMatrix,
			bool IsWindow,bool IsVertical,float order,bool IsXAxis);

	public:
		BillMeshGenerator() = default;
		virtual ~BillMeshGenerator() = default;
		void Generate(std::vector<std::vector<float>>& VertexData, std::vector<int>& Dimention, std::vector<unsigned short>& Indices);
	};
}