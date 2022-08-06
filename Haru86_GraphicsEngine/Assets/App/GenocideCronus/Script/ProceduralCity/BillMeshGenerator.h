#pragma once
#include <vector>
#include <memory>
#include <glm/glm.hpp>

class Mesh;

namespace myapp {
	class BillMeshGenerator
	{
	private:
		static void PrepareBoxVertexData(std::vector<std::vector<float>>& VertexData, std::vector<unsigned short>& Indices, glm::mat4 LocalTransMatrix);
	public:
		static void Generate(std::vector<std::vector<float>>& VertexData, std::vector<int>& Dimention, std::vector<unsigned short>& Indices);
	};
}