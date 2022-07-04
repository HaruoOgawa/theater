#include "RealtimeReflectionProbe.h"
#include "GraphicsEngine/Graphics/Texture.h"
#include "GraphicsEngine/Graphics/GraphicsRenderer.h"
#include "GraphicsEngine/Component/TransformComponent.h"

RealtimeReflectionProbe::RealtimeReflectionProbe():
	m_CubeTex(std::make_shared<Texture>()),
	m_FramebufferIndex(0)

{
	m_CubeCameraTRS.resize(6, std::make_shared<TransformComponent>());
	Start();

	//
	//m_CubeTexList.resize(6, std::make_shared<Texture>());
	//m_FramebufferIndexList.resize(6, 0);
	//
}

void RealtimeReflectionProbe::Start() {
	//
	GraphicsRenderer::GetInstance()->CreateFrameBuffer(GraphicsRenderer::GetInstance()->GetScreenSize().x, GraphicsRenderer::GetInstance()->GetScreenSize().y,
		m_CubeTex, m_FramebufferIndex, GL_RGBA, GL_RGBA,GL_UNSIGNED_BYTE,ERenderTargetType::REALTIME_CUBEMAP);
	
	float RP_Size = 50.0f;
	m_CubeCameraTRS[0]->m_center = glm::vec3(1.0 * RP_Size, 0.0, 0.0);
	m_CubeCameraTRS[1]->m_center = glm::vec3(-1.0 * RP_Size, 0.0, 0.0);
	m_CubeCameraTRS[2]->m_center = glm::vec3(0.0, 1.0 * RP_Size, 0.0);
	m_CubeCameraTRS[3]->m_center = glm::vec3(0.0, -1.0 * RP_Size, 0.0);
	m_CubeCameraTRS[4]->m_center = glm::vec3(0.0, 0.0, 1.0 * RP_Size);
	m_CubeCameraTRS[5]->m_center = glm::vec3(0.0, 0.0, -1.0 * RP_Size);

	GraphicsMain::GetInstance()->m_CameraTransformList.push_back(m_CubeCameraTRS[0]);
	GraphicsMain::GetInstance()->m_CameraTransformList.push_back(m_CubeCameraTRS[1]);
	GraphicsMain::GetInstance()->m_CameraTransformList.push_back(m_CubeCameraTRS[2]);
	GraphicsMain::GetInstance()->m_CameraTransformList.push_back(m_CubeCameraTRS[3]);
	GraphicsMain::GetInstance()->m_CameraTransformList.push_back(m_CubeCameraTRS[4]);
	GraphicsMain::GetInstance()->m_CameraTransformList.push_back(m_CubeCameraTRS[5]);

	//
	/*for (int i = 0; i < 6; i++) {
		GraphicsRenderer::GetInstance()->CreateFrameBuffer(GraphicsRenderer::GetInstance()->GetScreenSize().x, GraphicsRenderer::GetInstance()->GetScreenSize().y,
			m_CubeTexList[i], m_FramebufferIndexList[i], GL_RGBA, GL_RGBA);
	}*/
	
	//
}

void RealtimeReflectionProbe::Update() {

}

void RealtimeReflectionProbe::Draw() {
	std::vector<std::function<void(void)>> callbackList;
	callbackList.push_back([this]() {glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X, m_CubeTex->GetTextureID(), 0); });
	callbackList.push_back([this]() {glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_NEGATIVE_X, m_CubeTex->GetTextureID(), 0); });
	callbackList.push_back([this]() {glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_Y, m_CubeTex->GetTextureID(), 0); });
	callbackList.push_back([this]() {glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, m_CubeTex->GetTextureID(), 0); });
	callbackList.push_back([this]() {glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_Z, m_CubeTex->GetTextureID(), 0); });
	callbackList.push_back([this]() {glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, m_CubeTex->GetTextureID(), 0); });

	for (int drawIndex = 0; drawIndex < 6; drawIndex++) {
		GraphicsMain::GetInstance()->m_UseCameraIndex = drawIndex+1;
		GraphicsRenderer::GetInstance()->Draw(m_FramebufferIndex, callbackList[drawIndex]);
	}

	//
	/*for (int drawIndex = 0; drawIndex < 6; drawIndex++) {
		GraphicsMain::GetInstance()->m_UseCameraIndex = drawIndex+1;
		GraphicsRenderer::GetInstance()->Draw(m_FramebufferIndexList[drawIndex], []() {});
	}*/
	//
}