#pragma once
#include <memory>
#include <vector>
#include <glm/glm.hpp>

namespace myapp {
	class Flower;
	class Stem;
	class Leaf;

	struct BSplineData
	{
		std::vector<glm::vec3> controlPoints;
		float knotMin;
		float knotMax;
		float tWidth;

		BSplineData(const std::vector<glm::vec3>& points, float min, float max, float tw) {
			this->controlPoints = points;
			this->knotMin = min;
			this->knotMax = max;
			this->tWidth = tw;
		}
	};

	class FlowerModel
	{
		//
		int count;
		bool flowersIsDone;
		bool stemIsDone;
		bool leafIsDone;

		//
		std::shared_ptr<Flower> m_Flower;
		std::shared_ptr<Stem> m_Stem;
		std::shared_ptr<Leaf> m_Leaf;
	public:
		FlowerModel();
		~FlowerModel() = default;
		void Start();
		void Update();
		void Draw();

		friend Flower;
		friend Stem;
		friend Leaf;
	};
}
