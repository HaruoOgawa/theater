#pragma once
#include <memory>

class Mesh;
class Material;
class TransformComponent;
class GameObject;
class MeshRendererComponent;
class ReflectionProbe;

namespace myapp {
	enum class BillRPProgress
	{
		Initialize,
		Draw,
		Separation,
		None,
	};

	class ProceduralCity
	{
		// デバッグ用
		std::shared_ptr<GameObject> m_GridPlane;
		std::shared_ptr<MeshRendererComponent> m_DebugSphere;
		
		// リフレクションプローブ関連
		std::shared_ptr<ReflectionProbe> m_BillRP;
		BillRPProgress m_BillRPProgress;
		int m_RPDrawCount;

		// 3Dオブジェクト
		std::shared_ptr<MeshRendererComponent> m_Mandelbox;
		std::shared_ptr<MeshRendererComponent> m_BillMeshRenderer4Instanced;
		std::shared_ptr<MeshRendererComponent> m_ProceduralBillRenderer;
		std::shared_ptr<MeshRendererComponent> m_Street;
	public:
		ProceduralCity();
		virtual ~ProceduralCity()=default;
		void Start();
		void Update();
		void Draw(bool IsRaymarching);
	};
}

