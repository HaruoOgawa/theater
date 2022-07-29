#pragma once
#include <memory>

class GameObject;

namespace myapp 
{
	class FlowerModel;
	class LTreeModel;

	class Forest
	{
		std::shared_ptr<FlowerModel> m_FlowerModel;
		std::shared_ptr<LTreeModel> m_LTreeModel;
		std::shared_ptr<GameObject> m_EnergyBall;
		std::shared_ptr<GameObject> m_Ground;
	public:
		Forest();
		~Forest() = default;
		void Start();
		void Update();
		void Draw();
	};
}