#pragma once

#include <memory>

class Mesh;
class Material;
class TransformComponent;
class GameObject;
class ReflectionProbe;

namespace myapp {
	class SacredLake
	{
		//
		std::shared_ptr<ReflectionProbe> m_RP;
		std::shared_ptr<ReflectionProbe> m_MonoDirRP;

		// 
		std::shared_ptr<GameObject> m_Mandelbox;
		std::shared_ptr<GameObject> m_VolumetricCloud;

		//
		std::shared_ptr<Material> m_ReflectPlaneMaterial;
		std::shared_ptr<TransformComponent> m_ReflectPlaneTRS;
		std::shared_ptr<Mesh> m_ReflectPlaneMesh;

		std::shared_ptr<TransformComponent> m_ReflectSphereTRS;
		std::shared_ptr<Mesh> m_ReflectSphereMesh;

		// GPU Object
		std::shared_ptr<Material> m_GPUMaterial;
		std::shared_ptr<TransformComponent> m_GPUTRS;
		std::shared_ptr<Mesh> m_GPUParticleMesh;

	public:
		SacredLake();
		virtual ~SacredLake() = default;

		void Start();
		void Update();
		void Draw();
	};
}