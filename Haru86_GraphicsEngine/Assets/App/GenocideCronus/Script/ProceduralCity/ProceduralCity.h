#pragma once
#include <memory>

class Mesh;
class Material;
class TransformComponent;
class GameObject;
class MeshRendererComponent;

namespace myapp {

	class ProceduralCity
	{
		// デバッグ用
		std::shared_ptr<GameObject> m_GridPlane;

		std::shared_ptr<MeshRendererComponent> m_Mandelbox;
		std::shared_ptr<MeshRendererComponent> m_BillMeshRenderer4Instanced;
		
		// 
		//std::shared_ptr<MeshRendererComponent> m_ProceduralBillRenderer;
	public:
		ProceduralCity();
		virtual ~ProceduralCity()=default;
		void Start();
		void Update();
		void Draw(bool IsRaymarching);
	};
}

