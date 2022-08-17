#include "ReflectionProbe.h"
#include "GraphicsEngine/Graphics/Texture.h"
#include "GraphicsEngine/Graphics/GraphicsRenderer.h"
#include "GraphicsEngine/Component/TransformComponent.h"
#include "GraphicsEngine/Message/Console.h"

ReflectionProbe::ReflectionProbe(glm::vec3 Offset, float Size):
	m_CubeTex(std::make_shared<Texture>()),
	m_FramebufferIndex(0),
	m_ReflectionType(EReflectionType::CUBEMAP),
	m_Offset(Offset),
	m_Size(Size),
	m_Progress(EReflectionProbeProgress::Initialize),
	m_RPDrawCount(0)
{
	for (int i = 0; i < 6; i++)
	{
		m_CubeCameraTRS.push_back(std::make_shared<TransformComponent>());
	}

	Start();
}

ReflectionProbe::ReflectionProbe(std::shared_ptr<TransformComponent> TRS) :
	m_CubeTex(std::make_shared<Texture>()),
	m_FramebufferIndex(0),
	m_ReflectionType(EReflectionType::MONODIRECTIONAL),
	m_Offset(glm::vec3(0.0f)),
	m_Size(50.0f),
	m_Progress(EReflectionProbeProgress::Initialize),
	m_RPDrawCount(0)
{
	m_CubeCameraTRS.push_back(TRS);
	Start();
}

void ReflectionProbe::Start() {
	if (m_ReflectionType==EReflectionType::CUBEMAP) {
		//
		GraphicsRenderer::GetInstance()->CreateFrameBuffer(1024, 1024,
			m_CubeTex, m_FramebufferIndex, GL_RGBA, GL_RGBA, GL_UNSIGNED_BYTE, ERenderTargetType::REALTIME_CUBEMAP);

		glm::vec3 offset = m_Offset;
		float RP_Size = m_Size;
		m_CubeCameraTRS[0]->m_position = glm::vec3(1.0 * RP_Size * 0.5, 0.0, 0.0) + offset;
		m_CubeCameraTRS[1]->m_position = glm::vec3(-1.0 * RP_Size * 0.5, 0.0, 0.0) + offset;
		m_CubeCameraTRS[2]->m_position = glm::vec3(0.0, -1.0 * RP_Size * 0.5, 0.0) + offset;
		m_CubeCameraTRS[3]->m_position = glm::vec3(0.0, 1.0 * RP_Size * 0.5, 0.0) + offset;
		m_CubeCameraTRS[4]->m_position = glm::vec3(0.0, 0.0, 1.0 * RP_Size * 0.5) + offset;
		m_CubeCameraTRS[5]->m_position = glm::vec3(0.0, 0.0, -1.0 * RP_Size * 0.5) + offset;

		m_CubeCameraTRS[0]->m_center += offset;
		m_CubeCameraTRS[1]->m_center += offset;
		m_CubeCameraTRS[2]->m_center += offset;
		m_CubeCameraTRS[3]->m_center += offset;
		m_CubeCameraTRS[4]->m_center += offset;
		m_CubeCameraTRS[5]->m_center += offset;

		m_CubeCameraTRS[0]->m_up = glm::vec3(0.0f, -1.0f, 0.0f);
		m_CubeCameraTRS[1]->m_up = glm::vec3(0.0f, -1.0f, 0.0f);
		m_CubeCameraTRS[2]->m_up = glm::vec3(0.0f, 0.0f, -1.0f);
		m_CubeCameraTRS[3]->m_up = glm::vec3(0.0f, 0.0f, 1.0f);
		m_CubeCameraTRS[4]->m_up = glm::vec3(0.0f, -1.0f, 0.0f);
		m_CubeCameraTRS[5]->m_up = glm::vec3(0.0f, -1.0f, 0.0f);
	}
	else if (m_ReflectionType == EReflectionType::MONODIRECTIONAL) {
		GraphicsRenderer::GetInstance()->CreateFrameBuffer(1024, 1024,
			m_CubeTex, m_FramebufferIndex, GL_RGBA, GL_RGBA, GL_UNSIGNED_BYTE, ERenderTargetType::COLOR_TEXTURE_BUFFER);
	}
}

void ReflectionProbe::Update() 
{
	switch (m_Progress)
	{
	case EReflectionProbeProgress::Initialize:
		GraphicsMain::GetInstance()->m_ReflectionProbeList.push_back(this);

		m_Progress = EReflectionProbeProgress::Draw;
		break;
	case EReflectionProbeProgress::Draw:
		m_RPDrawCount++;

		if (m_RPDrawCount > 2)m_Progress = EReflectionProbeProgress::Separation;
		break;
	case EReflectionProbeProgress::Separation:
	{
		auto& RPList = GraphicsMain::GetInstance()->m_ReflectionProbeList;
		auto Item = std::find(RPList.begin(), RPList.end(), this);
		if (Item != RPList.end())
		{
			RPList.erase(RPList.begin() + std::distance(RPList.begin(), Item));
		}

		m_Progress = EReflectionProbeProgress::End;
		break;
	}
	case EReflectionProbeProgress::End:
	case EReflectionProbeProgress::None:
		break;
	default:
		break;
	}
}

void ReflectionProbe::Draw() {
	if (m_ReflectionType==EReflectionType::CUBEMAP) 
	{
		std::vector<std::function<void(void)>> callbackList;
		callbackList.push_back([this]() {glBindTexture(GL_TEXTURE_CUBE_MAP, m_CubeTex->GetTextureID()); glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X, m_CubeTex->GetTextureID(), 0); });
		callbackList.push_back([this]() {glBindTexture(GL_TEXTURE_CUBE_MAP, m_CubeTex->GetTextureID()); glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_NEGATIVE_X, m_CubeTex->GetTextureID(), 0); });
		callbackList.push_back([this]() {glBindTexture(GL_TEXTURE_CUBE_MAP, m_CubeTex->GetTextureID()); glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_Y, m_CubeTex->GetTextureID(), 0); });
		callbackList.push_back([this]() {glBindTexture(GL_TEXTURE_CUBE_MAP, m_CubeTex->GetTextureID()); glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, m_CubeTex->GetTextureID(), 0); });
		callbackList.push_back([this]() {glBindTexture(GL_TEXTURE_CUBE_MAP, m_CubeTex->GetTextureID()); glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_Z, m_CubeTex->GetTextureID(), 0); });
		callbackList.push_back([this]() {glBindTexture(GL_TEXTURE_CUBE_MAP, m_CubeTex->GetTextureID()); glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, m_CubeTex->GetTextureID(), 0); });

		for (int drawIndex = 0; drawIndex < 6; drawIndex++) {
			GraphicsRenderer::GetInstance()->Draw(m_CubeCameraTRS[drawIndex],false,m_FramebufferIndex, callbackList[drawIndex], m_CubeTex->GetWidth(), m_CubeTex->GetHeight());
		}
	}
	else if (m_ReflectionType == EReflectionType::MONODIRECTIONAL) 
	{
		GraphicsRenderer::GetInstance()->Draw(m_CubeCameraTRS[0], true, m_FramebufferIndex, []() {}, m_CubeTex->GetWidth(), m_CubeTex->GetHeight());
	}
}