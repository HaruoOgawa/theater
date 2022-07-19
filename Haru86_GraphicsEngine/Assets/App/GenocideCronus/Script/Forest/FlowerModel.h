#pragma once
#include <memory>

namespace myapp {
	class Flower;
	class Stem;
	class Leaf;

	class FlowerModel
	{
		std::shared_ptr<Flower> m_Flower;
		std::shared_ptr<Stem> m_Stem;
		std::shared_ptr<Leaf> m_Leaf;
	public:
		FlowerModel();
		~FlowerModel() = default;
		void Start();
		void Update();
		void Draw();

		/*friend Flower;
		friend Stem;
		friend Leaf;*/
	};
}
