#pragma once
#include <memory>

class MeshRendererComponent;

namespace myapp {
	class SacredLake
	{
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
	};
}