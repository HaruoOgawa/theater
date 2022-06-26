#include "SoundShaderPlayer.h"
#include "GraphicsEngine/Graphics/Mesh.h"
#include "GraphicsEngine/Graphics/Material.h"
#include "GraphicsEngine/Graphics/Texture.h"
#include "GraphicsEngine/Graphics/ShaderLib.h"
#include "GraphicsEngine/Graphics/GraphicsRenderer.h"
#include "GraphicsEngine/GraphicsMain/GraphicsMain.h"

#ifdef _DEBUG
#include "GraphicsEngine/Message/Console.h"
#endif // _DEBUG


namespace sound {
	SoundShaderPlayer::SoundShaderPlayer(const std::string& soundCode):
		m_FrameTex(nullptr),
		m_FrameIndex(0)
	{
		m_Mesh = std::make_shared<Mesh>(PrimitiveType::BOARD);
		m_Material = std::make_shared<Material>(RenderingSurfaceType::None, shaderlib::ShaderLib::StandardRenderBoard_vert, soundCode);
		m_FrameTex = std::make_shared<Texture>();
		GraphicsRenderer::GetInstance()->CreateFrameBuffer(m_FrameTex, m_FrameIndex, GL_RGBA16F, GL_RGBA, GL_FLOAT);
	}

	bool SoundShaderPlayer::Initialize() {
		DrawSound();
		return true;
	}

	bool SoundShaderPlayer::Update() {
#ifdef _DEBUG
		Console::Log("m_SoundData.size(): %d\n", m_SoundData.size());
		Console::Log("m_SoundData=> [0]:%f, [1]:%f, [2]:%f\n", static_cast<float>(m_SoundData[0]), static_cast<float>(m_SoundData[1]), static_cast<float>(m_SoundData[2]));
#endif // _DEBUG

		return true;
	}

	void SoundShaderPlayer::DrawSound() {
		GraphicsMain::GetInstance()->renderingTarget = ERerderingTarget::COLOR;
		glBindFramebuffer(GL_FRAMEBUFFER, m_FrameIndex);
		glViewport(0, 0, static_cast<int>(GraphicsRenderer::GetInstance()->GetScreenSize().x * GraphicsRenderer::GetInstance()->frameResolusion),
			static_cast<int>(GraphicsRenderer::GetInstance()->GetScreenSize().y * GraphicsRenderer::GetInstance()->frameResolusion));
		
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		m_Material->SetActive();
		m_Material->SetFloatUniform("_frameResolusion", GraphicsRenderer::GetInstance()->frameResolusion);

		m_Mesh->Draw();

		ReadSoundBuffer();
	}

	void SoundShaderPlayer::ReadSoundBuffer() {
		int x = static_cast<int>(GraphicsRenderer::GetInstance()->GetScreenSize().x * GraphicsRenderer::GetInstance()->frameResolusion);
		int y = static_cast<int>(GraphicsRenderer::GetInstance()->GetScreenSize().y * GraphicsRenderer::GetInstance()->frameResolusion);

		m_SoundData.clear();
		m_SoundData.resize(x * y * 4);

		//char[x * y * 4] buf;

		//glReadBuffer(GL_FRONT);

		glReadPixels(
			0,
			0,
			x,
			y,
			GL_RGBA,
			GL_FLOAT,
			&m_SoundData[0]
		);
	}
}