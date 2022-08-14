#pragma once
#include <memory>
#include <glm/glm.hpp>

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
		// リフレクションプローブ関連
		std::shared_ptr<ReflectionProbe> m_BillRP;
		BillRPProgress m_BillRPProgress;
		int m_RPDrawCount;

		// 3Dオブジェクト
		std::shared_ptr<MeshRendererComponent> m_Mandelbox;
		std::shared_ptr<MeshRendererComponent> m_BillMeshRenderer4Instanced;
		std::shared_ptr<MeshRendererComponent> m_ProceduralBillRenderer;
		std::shared_ptr<MeshRendererComponent> m_Street;
		glm::vec3							   m_XSideWarkVec;
	public:
		ProceduralCity();
		virtual ~ProceduralCity()=default;
		void Start();
		void Update();
		void Draw(bool IsRaymarching);
	};
}

