#include "RealtimeReflectionProbe.h"
#include "GraphicsEngine/Graphics/Texture.h"
#include "GraphicsEngine/Graphics/GraphicsRenderer.h"
#include "GraphicsEngine/Component/TransformComponent.h"

RealtimeReflectionProbe::RealtimeReflectionProbe(){
	m_CubeTexList.resize(6, std::make_shared<Texture>());
	m_FramebufferIndexList.resize(6, 0);
	m_CubeCameraTRS.resize(6, std::make_shared<TransformComponent>());

	Start();
}

void RealtimeReflectionProbe::Start() {
	//
	for (int i = 0; i < 6; i++) {
		GraphicsRenderer::GetInstance()->CreateFrameBuffer(GraphicsRenderer::GetInstance()->GetScreenSize().x, GraphicsRenderer::GetInstance()->GetScreenSize().y,
			m_CubeTexList[i], m_FramebufferIndexList[i], GL_RGBA, GL_RGBA);

		GraphicsMain::GetInstance()->m_CameraTransformList.push_back(m_CubeCameraTRS[i]);
	}

	float RP_Size = 50.0f;
	m_CubeCameraTRS[0]->m_center = glm::vec3(1.0 * RP_Size, 0.0, 0.0);
	m_CubeCameraTRS[1]->m_center = glm::vec3(-1.0 * RP_Size, 0.0, 0.0);
	m_CubeCameraTRS[2]->m_center = glm::vec3(0.0, 1.0 * RP_Size, 0.0);
	m_CubeCameraTRS[3]->m_center = glm::vec3(0.0, -1.0 * RP_Size, 0.0);
	m_CubeCameraTRS[4]->m_center = glm::vec3(0.0, 0.0, 1.0 * RP_Size);
	m_CubeCameraTRS[5]->m_center = glm::vec3(0.0, 0.0, -1.0 * RP_Size);
}

void RealtimeReflectionProbe::Update() {

}

void RealtimeReflectionProbe::Draw() {
	for (int drawIndex=0; drawIndex < 6;drawIndex++) {
		GraphicsMain::GetInstance()->m_UseCameraIndex = drawIndex;
		GraphicsRenderer::GetInstance()->Draw(m_FramebufferIndexList[drawIndex]);
	}
}