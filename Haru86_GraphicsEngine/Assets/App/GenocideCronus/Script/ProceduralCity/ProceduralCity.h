#pragma once
#include <memory>
#include <glm/glm.hpp>

class MeshRendererComponent;
class ReflectionProbe;

namespace myapp {
	class ProceduralCity
	{
		// リフレクションプローブ
		std::shared_ptr<ReflectionProbe> m_BillRP;
		
		// 3Dオブジェクト
		std::shared_ptr<MeshRendererComponent> m_ProceduralBillRenderer;
		std::shared_ptr<MeshRendererComponent> m_Street;
		glm::vec3							   m_XSideWarkVec;
		std::shared_ptr<MeshRendererComponent> m_StreeLamp;
		std::shared_ptr<MeshRendererComponent> m_CylinderBill;
		std::shared_ptr<MeshRendererComponent> m_GaffDoor;
		std::shared_ptr<MeshRendererComponent> m_RubbleParticle;

		//
		int NumOfProBill;
		int NumOfCyBill;

		//
		std::shared_ptr<MeshRendererComponent> m_Mandelbox;
		std::shared_ptr<MeshRendererComponent> m_CityCloud;

		// Timeline Val
		bool m_IsDrawMandel;
		bool m_IsDrawCloud;
		bool m_IsDownSideCloud;
		bool m_IsUseBloom;
	public:
		ProceduralCity();
		virtual ~ProceduralCity()=default;
		void Start();
		void Update();
		void Draw(bool IsRaymarching, int LinearInstanceRate);
		void UpdateTimeline(float LocalTime, bool IsReverseTime);
	};
}

