#pragma once

namespace myapp {
	class FlowerModel;
	class Stem;

	class Leaf
	{
		//
		FlowerModel* m_FlowerModel;
	public:
		Leaf(FlowerModel* model);
		~Leaf() = default;
		void Start();
		void LinkBufferToResources(const std::shared_ptr<Stem>& stem);
		void Update();
		void Draw();
	};
}