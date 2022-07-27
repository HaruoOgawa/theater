#pragma once
#include <memory>

namespace myapp 
{
	class FlowerModel;
	class LTreeModel;

	class Forest
	{
		std::shared_ptr<FlowerModel> m_FlowerModel;
		std::shared_ptr<LTreeModel> m_LTreeModel;
	public:
		Forest();
		~Forest() = default;
		void Start();
		void Update();
		void Draw();
	};
}