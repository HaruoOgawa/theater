#pragma once
#include <vector>
#include <memory>
#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>

class Mesh;
class Material;
class TransformComponent;

namespace myapp
{
	// â‘(â‘Ç—ÇÁÇÃèWçá)
	struct Multi_Flower_Data {
		std::vector<glm::vec3> vertices;
		std::vector<glm::vec3> normals;
		std::vector<unsigned short> triangles;
	};

	// â‘Ç—ÇÁ
	struct BaseFlower_Data
	{
		std::vector<glm::vec3> vertices;
		std::vector<glm::vec3> normals;
		std::vector<unsigned short> triangles;
	};

	struct B_Spline_Data {
		glm::vec3 position;
		int index;

		B_Spline_Data(glm::vec3 p, int i) {
			this->position = p;
			this->index = i;
		}
	};

	class Flower
	{
		std::shared_ptr<Mesh> m_FlowerMesh;
		std::shared_ptr<Material> m_FlowerMaterial;
		std::shared_ptr<TransformComponent> m_FlowerTRS;
	public:
		Flower();
		~Flower() = default;
		void Start();
		void Update();
		void Draw();

	private:
		// â‘ÇÃç\íz
		static std::shared_ptr<Multi_Flower_Data> RenderMultiFlower(const std::shared_ptr<BaseFlower_Data>& flower_data, glm::vec3 flowerPosition,
			glm::vec3 flowerTangent, glm::vec3 flowerBioNormal, float flowerTime = 1.0f, int N = 50);

		static void CalFibonacciPosition(std::vector<glm::vec3>& FibonacciPosition, std::vector<glm::quat>& FibonacciRotation, std::vector<glm::vec4>& FibonacciGrowthData, int N = 50);

		// â‘Ç—ÇÁä÷òA
		static std::shared_ptr<BaseFlower_Data> Cal_BSpline_Surface(std::vector<glm::vec3> controlPoints, float knotMin, float knotMax, float tWidth = 0.01f);
		static std::vector<std::shared_ptr<B_Spline_Data>> Cal_BSplineCurve(std::vector<glm::vec3> controlPoints, float knotMin, float knotMax, float tWidth = 0.01f);
		static std::vector<float> GetKnotVector(int m, int n, float knotMin, float knotMax);
		static float GetBasisFunction(std::vector<float> u, int j, int k, float t);
		std::vector<float> CastVec3ToLine_float(std::vector<glm::vec3> BlockVector) {
			std::vector<float> result;
			for (const auto& block : BlockVector) {
				result.push_back(block.x);
				result.push_back(block.y);
				result.push_back(block.z);
			}

			return result;
		}
	};

}
