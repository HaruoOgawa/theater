#pragma once

#include <memory>

class Texture;
class Mesh;
class Material;
class TransformComponent;
class GameObject;

namespace myapp {
	class SacredLake
	{
		// 
		std::shared_ptr<GameObject> m_Mandelbox;
		std::shared_ptr<GameObject> m_ReflectPlane;

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