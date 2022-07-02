#pragma once
#include <memory>

class Texture;
class Mesh;
class Material;
class TransformComponent;
class GameObject;

namespace myapp {
	class BillWindowGenerator;

	class ProceduralCity
	{
		//
		std::shared_ptr<GameObject> m_Mandelbox;
		std::shared_ptr<Mesh>        m_BillMesh4Instanced;
		std::shared_ptr<Material>    m_BillMaterial4Instanced;
		std::shared_ptr<TransformComponent> m_transform;
		std::shared_ptr<BillWindowGenerator> m_BillWindowGenerator;

	public:
		ProceduralCity();
		virtual ~ProceduralCity()=default;
		void Start();
		void Update();
		void Draw();
	};
}

