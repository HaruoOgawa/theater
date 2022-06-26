#include "TheaterDemo.h"

#include "GraphicsEngine/GraphicsMain/CTimeline.h"
#include "GraphicsEngine/Object/GameObject.h"
#include "GraphicsEngine/Graphics/ShaderLib.h"
#include "GraphicsEngine/Component/TransformComponent.h"
#include "GraphicsEngine/Sound/SoundShaderPlayer.h"

#define PI 3.0f

void TheaterDemo::Start() {
	// サウンド
	std::string soundCode = {
		#include "Shader/Sound.frag"
	};
	m_SoundPlayer = std::make_shared<sound::SoundShaderPlayer>(soundCode);
	m_SoundPlayer->Initialize();

	// 背景色
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
	m_GridPlane->m_transform->m_rotation= glm::vec3(PI / 2.0f, 0.0, 0.0);
	m_GridPlane->m_transform->m_scale= glm::vec3(25.0f);

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
	// サウンド
	m_SoundPlayer->Update();
}

void TheaterDemo::Draw() {

}

void TheaterDemo::Timeline(CTimeline* timeline) {

}