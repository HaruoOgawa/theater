#include "TheaterDemo.h"

#include "GraphicsEngine/GraphicsMain/CTimeline.h"
#include "GraphicsEngine/Object/GameObject.h"
#include "GraphicsEngine/Graphics/ShaderLib.h"

#define PI 3.0f

void TheaterDemo::Start() {
	// ”wŒiF
	GraphicsRenderer::GetInstance()->SetBackgroudColor(glm::vec4(glm::vec3(0.85f), 1.0));

	//
	m_GridPlane = std::make_shared<GameObject>(
		PrimitiveType::BOARD,
		RenderType::DefaultBuffer,
		RenderQueue::Geometry,
		RenderingSurfaceType::RASTERIZER,
		shaderlib::ShaderLib::Standard_vert,
		shaderlib::ShaderLib::GridPlane_frag
		);
	m_GridPlane->SetRotation(glm::vec3(PI / 2.0f, 0.0, 0.0));
	m_GridPlane->SetPosition(glm::vec3(0.0f, 0.0f, 0.0f));
	m_GridPlane->SetScale(glm::vec3(25.0f));

	// raymarching
	std::string sample = {
		#include "./Shader/GlowCave.frag"
	};

	m_Raymarching = std::make_shared<GameObject>(
		PrimitiveType::BOARD,
		RenderType::DefaultBuffer,
		RenderQueue::Geometry,
		RenderingSurfaceType::RAYMARCHING,
		shaderlib::ShaderLib::RaymarchingObject_vert,
		sample
	);

}

void TheaterDemo::Update() {

}

void TheaterDemo::Draw() {

}

void TheaterDemo::Timeline(CTimeline* timeline) {

}