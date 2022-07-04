#include "RealtimeReflectionProbe.h"
#include "GraphicsEngine/Graphics/Texture.h"
#include "GraphicsEngine/Graphics/GraphicsRenderer.h"
#include "GraphicsEngine/Component/TransformComponent.h"
#include "GraphicsEngine/Message/Console.h"

RealtimeReflectionProbe::RealtimeReflectionProbe():
	m_CubeTex(std::make_shared<Texture>()),
	m_FramebufferIndex(0)

{
	for (int i = 0; i < 6; i++) {
		//
		//m_CubeTexList.push_back(std::make_shared<Texture>());
		//m_FramebufferIndexList.push_back(0);
		//

		m_CubeCameraTRS.push_back(std::make_shared<TransformComponent>());
	}

	Start();
}

void RealtimeReflectionProbe::Start() {
	//
	GraphicsRenderer::GetInstance()->CreateFrameBuffer(1024, 1024,
		m_CubeTex, m_FramebufferIndex, GL_RGBA, GL_RGBA,GL_UNSIGNED_BYTE,ERenderTargetType::REALTIME_CUBEMAP);
	
	glm::vec3 offset = glm::vec3(0.0f, 10.0f, 0.0f);
	float RP_Size = 50.0f;
	m_CubeCameraTRS[0]->m_position = glm::vec3(1.0 * RP_Size, 0.0, 0.0)+ offset;
	m_CubeCameraTRS[1]->m_position = glm::vec3(-1.0 * RP_Size, 0.0, 0.0) + offset;
	m_CubeCameraTRS[2]->m_position = glm::vec3(0.0, 1.0 * RP_Size, 0.0) + offset;
	m_CubeCameraTRS[3]->m_position = glm::vec3(0.0, -1.0 * RP_Size, 0.0) + offset;
	m_CubeCameraTRS[4]->m_position = glm::vec3(0.0, 0.0, 1.0 * RP_Size) + offset;
	m_CubeCameraTRS[5]->m_position = glm::vec3(0.0, 0.0, -1.0 * RP_Size) + offset;

	m_CubeCameraTRS[0]->m_scale += offset;
	m_CubeCameraTRS[1]->m_scale += offset;
	m_CubeCameraTRS[2]->m_scale += offset;
	m_CubeCameraTRS[3]->m_scale += offset;
	m_CubeCameraTRS[4]->m_scale += offset;
	m_CubeCameraTRS[5]->m_scale += offset;

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
	/*Console::Log("m_CubeCameraTRS.size(): %d\n", m_CubeCameraTRS.size());
	Console::Log("m_CubeCameraTRS[2]=> x:%f, y:%f, z:%f\n", m_CubeCameraTRS[2]->m_center.x, m_CubeCameraTRS[2]->m_center.y, m_CubeCameraTRS[2]->m_center.z);

	Console::Log("GraphicsMain::GetInstance()->m_CameraTransformList.size(): %d\n", GraphicsMain::GetInstance()->m_CameraTransformList.size());
	Console::Log("GraphicsMain::GetInstance()->m_CameraTransformList[3]=> x:%f, y:%f, z:%f\n", GraphicsMain::GetInstance()->m_CameraTransformList[3]->m_center.x, GraphicsMain::GetInstance()->m_CameraTransformList[3]->m_center.y, GraphicsMain::GetInstance()->m_CameraTransformList[3]->m_center.z);
	*/

}

void RealtimeReflectionProbe::Update() {

}

void RealtimeReflectionProbe::Draw() {
	std::vector<std::function<void(void)>> callbackList;
	callbackList.push_back([this]() {glBindTexture(GL_TEXTURE_CUBE_MAP, m_CubeTex->GetTextureID()); glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X, m_CubeTex->GetTextureID(), 0); });
	callbackList.push_back([this]() {glBindTexture(GL_TEXTURE_CUBE_MAP, m_CubeTex->GetTextureID()); glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_NEGATIVE_X, m_CubeTex->GetTextureID(), 0); });
	callbackList.push_back([this]() {glBindTexture(GL_TEXTURE_CUBE_MAP, m_CubeTex->GetTextureID()); glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_Y, m_CubeTex->GetTextureID(), 0); });
	callbackList.push_back([this]() {glBindTexture(GL_TEXTURE_CUBE_MAP, m_CubeTex->GetTextureID()); glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, m_CubeTex->GetTextureID(), 0); });
	callbackList.push_back([this]() {glBindTexture(GL_TEXTURE_CUBE_MAP, m_CubeTex->GetTextureID()); glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_Z, m_CubeTex->GetTextureID(), 0); });
	callbackList.push_back([this]() {glBindTexture(GL_TEXTURE_CUBE_MAP, m_CubeTex->GetTextureID()); glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, m_CubeTex->GetTextureID(), 0); });

	for (int drawIndex = 0; drawIndex < 6; drawIndex++) {
		
		GraphicsMain::GetInstance()->m_UseCameraIndex = drawIndex+1;
		//Console::Log("m_FramebufferIndex: %d/ GraphicsMain::GetInstance()->m_UseCameraIndex: %d__________________________\n", m_FramebufferIndex, GraphicsMain::GetInstance()->m_UseCameraIndex);
		GraphicsRenderer::GetInstance()->Draw(m_FramebufferIndex, callbackList[drawIndex],m_CubeTex->GetWidth(),m_CubeTex->GetHeight());

		//Console::Log("___________________________________________________________________________________________\n");
	}
	
	//
	/*for (int drawIndex = 0; drawIndex < 6; drawIndex++) {
		GraphicsMain::GetInstance()->m_UseCameraIndex = drawIndex+1;
		GraphicsRenderer::GetInstance()->Draw(m_FramebufferIndexList[drawIndex], []() {});
	}*/
	//
}