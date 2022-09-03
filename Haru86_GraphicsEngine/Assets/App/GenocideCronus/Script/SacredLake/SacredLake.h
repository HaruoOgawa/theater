#pragma once
#include <memory>

class MeshRendererComponent;
class ReflectionProbe;

namespace myapp {
	class SacredLake
	{
		// リフレクションプローブ
		std::shared_ptr<ReflectionProbe> m_ReflectionProbe;

		//
		std::shared_ptr<MeshRendererComponent> m_Mandelbox;
		std::shared_ptr<MeshRendererComponent> m_VolumetricCloud;
		std::shared_ptr<MeshRendererComponent> m_ReflectPlane;
		std::shared_ptr<MeshRendererComponent> m_GPUParticle;
		std::shared_ptr<MeshRendererComponent> m_SphereGPUParticle;

	public:
		SacredLake();
		virtual ~SacredLake() = default;

		void Start();
		void Update();
		void Draw(bool IsRaymarching);
		void UpdateTimeline(float LocalTime, bool IsReverseTime);
	};
}