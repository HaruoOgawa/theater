#pragma once
#include <memory>

class GameObject;
class MeshRendererComponent;

namespace myapp 
{
	class FlowerModel;
	class LTreeModel;

	class Forest
	{
		std::shared_ptr<FlowerModel> m_FlowerModel;
		std::shared_ptr<LTreeModel> m_LTreeModel;

		std::shared_ptr<MeshRendererComponent> m_EnergyBall;
		std::shared_ptr<MeshRendererComponent> m_Ground;
		std::shared_ptr<MeshRendererComponent> m_AroundMountain;

		bool IsMountainRotate;
	public:
		Forest();
		~Forest() = default;
		void Start();
		void Update();
		void Draw(bool IsRaymarching,int SceneIndex, int LinearInstanceRate);
		void UpdateTimeline(float LocalTime);
	};
}