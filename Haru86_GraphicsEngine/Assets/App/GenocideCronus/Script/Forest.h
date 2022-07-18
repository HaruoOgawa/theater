#pragma once
#include <memory>

namespace myapp 
{
	class Flower;

	class Forest
	{
		std::shared_ptr<Flower> m_Flower;
	public:
		Forest();
		~Forest() = default;
		void Start();
		void Update();
		void Draw();
	};
}