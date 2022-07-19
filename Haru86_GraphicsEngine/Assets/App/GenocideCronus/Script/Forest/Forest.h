#pragma once
#include <memory>

namespace myapp 
{
	class FlowerModel;

	class Forest
	{
		std::shared_ptr<FlowerModel> m_FlowerModel;
	public:
		Forest();
		~Forest() = default;
		void Start();
		void Update();
		void Draw();
	};
}