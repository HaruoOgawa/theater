#include "SSR_Test.h"
#include "GraphicsEngine/Graphics/GraphicsRenderer.h"
#include "GraphicsEngine/Graphics/Mesh.h"
#include "GraphicsEngine/Graphics/Material.h"
#include "GraphicsEngine/Graphics/ShaderLib.h"
#include "GraphicsEngine/Component/TransformComponent.h"
#include "GraphicsEngine/Message/Console.h"
#include "GraphicsEngine/Object/GameObject.h"
#include "GraphicsEngine/Graphics/ReflectionProbe.h"
#include "GraphicsEngine/Graphics/Texture.h"
#include "GraphicsEngine/Component/MeshRendererComponent.h"

namespace myapp {
	SSR_Test::SSR_Test() :
		m_Plane(nullptr),
		m_Obj0(nullptr),
		m_Obj1(nullptr),
		m_Obj2(nullptr)
	{
		Start();
	}

	void SSR_Test::Start() {
		//
		m_Plane = std::make_shared<GameObject>(
			PrimitiveType::BOARD,
			RenderType::DefaultBuffer,
			RenderQueue::Geometry,
			RenderingSurfaceType::RASTERIZER,
			shaderlib::ShaderLib::Standard_vert,
			shaderlib::ShaderLib::Standard_frag
		);
		m_Plane->m_transform->m_rotation = glm::vec3(3.14f / 2.0f, 0.0f, 0.0f);
		m_Plane->m_transform->m_scale = glm::vec3(10.0f);
		m_Plane->meshComp->m_calllback = [this]() {
			m_Plane->meshComp->m_material->SetIntUniform("_UseColor", 1);
			m_Plane->meshComp->m_material->SetVec4Uniform("_Color", glm::vec4(0.1f, 0.1f, 0.1f, 1.0f));
		};

		//
		m_Obj0= std::make_shared<GameObject>(
			PrimitiveType::SPHERE,
			RenderType::DefaultBuffer,
			RenderQueue::Geometry,
			RenderingSurfaceType::RASTERIZER,
			shaderlib::ShaderLib::Standard_vert,
			shaderlib::ShaderLib::Standard_frag
		);
		m_Obj0->m_transform->m_position = glm::vec3(0.0f, 0.5f, -1.5f);
		m_Obj0->m_transform->m_scale = glm::vec3(0.75f);
		m_Obj0->meshComp->m_calllback = [this]() {
			m_Obj0->meshComp->m_material->SetIntUniform("_UseColor", 1);
			m_Obj0->meshComp->m_material->SetVec4Uniform("_Color", glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));
		};

		//
		m_Obj1 = std::make_shared<GameObject>(
			PrimitiveType::SPHERE,
			RenderType::DefaultBuffer,
			RenderQueue::Geometry,
			RenderingSurfaceType::RASTERIZER,
			shaderlib::ShaderLib::Standard_vert,
			shaderlib::ShaderLib::Standard_frag
			);
		m_Obj1->m_transform->m_position = glm::vec3(-1.0f, 0.5f, -0.75f);
		m_Obj1->m_transform->m_scale = glm::vec3(0.6f);
		m_Obj1->meshComp->m_calllback = [this]() {
			m_Obj1->meshComp->m_material->SetIntUniform("_UseColor", 1);
			m_Obj1->meshComp->m_material->SetVec4Uniform("_Color", glm::vec4(0.0f, 1.0f, 0.0f, 1.0f));
		};

		//
		m_Obj2 = std::make_shared<GameObject>(
			PrimitiveType::SPHERE,
			RenderType::DefaultBuffer,
			RenderQueue::Geometry,
			RenderingSurfaceType::RASTERIZER,
			shaderlib::ShaderLib::Standard_vert,
			shaderlib::ShaderLib::Standard_frag
			);
		m_Obj2->m_transform->m_position = glm::vec3(0.5f, 0.5f, -0.5f);
		m_Obj2->m_transform->m_scale = glm::vec3(0.4f);
		m_Obj2->meshComp->m_calllback = [this]() {
			m_Obj2->meshComp->m_material->SetIntUniform("_UseColor", 1);
			m_Obj2->meshComp->m_material->SetVec4Uniform("_Color", glm::vec4(0.0f, 0.0f, 1.0f, 1.0f));
		};
	}

	void SSR_Test::Update() {

	}

	void SSR_Test::Draw() {

	}
}