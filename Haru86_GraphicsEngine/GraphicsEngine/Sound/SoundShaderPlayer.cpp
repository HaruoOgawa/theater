#include "SoundShaderPlayer.h"
#include "GraphicsEngine/Graphics/Mesh.h"
#include "GraphicsEngine/Graphics/Material.h"
#include "GraphicsEngine/Graphics/Texture.h"
#include "GraphicsEngine/Graphics/ShaderLib.h"
#include "GraphicsEngine/Graphics/GraphicsRenderer.h"
#include "GraphicsEngine/GraphicsMain/GraphicsMain.h"

#include <Windows.h>
//#include <mmeapi.h>

#ifdef _DEBUG
#include "GraphicsEngine/Message/Console.h"
#endif // _DEBUG

namespace sound {
	SoundShaderPlayer::SoundShaderPlayer(const std::string& soundCode):
		m_FrameTex(nullptr),
		m_FrameIndex(0)
	{
		m_Mesh = std::make_shared<Mesh>(PrimitiveType::BOARD);
		m_Material = std::make_shared<Material>(RenderingSurfaceType::RASTERIZER, shaderlib::ShaderLib::StandardRenderBoard_vert, soundCode);
		m_FrameTex = std::make_shared<Texture>();
		GraphicsRenderer::GetInstance()->CreateFrameBuffer(m_FrameTex, m_FrameIndex, GL_RGBA16F, GL_RGBA, GL_FLOAT);
	}

	bool SoundShaderPlayer::Initialize() {
		//
		DrawSound();

		// mmeapi sound apiの準備
		HWAVEOUT hWaveOut;
		HGLOBAL hWaveHdr;
		LPWAVEHDR IpWaveHdr; // たぶんこれがサウンドデータを取り扱うやつ
		HANDLE hFormat;
		WAVEFORMAT* pFormat;

		// データを渡す
		hWaveHdr = GlobalAlloc(GMEM_MOVEABLE | GMEM_SHARE,
			(DWORD)sizeof(WAVEHDR));

		IpWaveHdr = (LPWAVEHDR)GlobalLock(hWaveHdr);
		memset(IpWaveHdr[0].lpData, 0,m_SoundDataL.size());
		memset(IpWaveHdr[1].lpData, 0,m_SoundDataR.size());

		memcpy(&IpWaveHdr[0].lpData[0], &m_SoundDataL[0], m_SoundDataL.size());
		memcpy(&IpWaveHdr[1].lpData[0], &m_SoundDataR[0], m_SoundDataR.size());

		//
		waveOutOpen((LPHWAVEOUT)&hWaveOut, WAVE_MAPPER, (LPWAVEFORMATEX)&pFormat,NULL, 0, CALLBACK_WINDOW);
		waveOutPrepareHeader(hWaveOut, IpWaveHdr,sizeof(WAVEHDR));
		waveOutWrite(hWaveOut, IpWaveHdr, sizeof(WAVEHDR));

		return true;
	}

	bool SoundShaderPlayer::Update() {
#ifdef _DEBUG
		//Console::Log("m_SoundDataL=> [0]:%f, [1]:%f / m_SoundDataR=> [0]:%f, [1]:%f\n",
		//	static_cast<float>(m_SoundDataL[0]), static_cast<float>(m_SoundDataL[1]), static_cast<float>(m_SoundDataR[0]), static_cast<float>(m_SoundDataR[1]));
#endif // _DEBUG

		// サウンドを再生する
		// waveOutGetPosition

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
		
		m_Material->SetVec2Uniform("_resolution", GraphicsRenderer::GetInstance()->GetScreenSize());
		m_Material->SetFloatUniform("_frameResolusion", GraphicsRenderer::GetInstance()->frameResolusion);
		if (GraphicsMain::GetInstance()->renderingTarget == ERerderingTarget::COLOR) {
			m_Material->SetFloatUniform("_RenderingTarget", 1.0);
		}
		else if (GraphicsMain::GetInstance()->renderingTarget == ERerderingTarget::DEPTH) {
			m_Material->SetFloatUniform("_RenderingTarget", 2.0);
		}
		m_Material->SetFloatUniform("_frameResolusion", GraphicsRenderer::GetInstance()->frameResolusion);

		m_Mesh->Draw();

		ReadSoundBuffer();
	}

	void SoundShaderPlayer::ReadSoundBuffer() {
		int x = static_cast<int>(GraphicsRenderer::GetInstance()->GetScreenSize().x * GraphicsRenderer::GetInstance()->frameResolusion);
		int y = static_cast<int>(GraphicsRenderer::GetInstance()->GetScreenSize().y * GraphicsRenderer::GetInstance()->frameResolusion);

		m_SoundDataL.resize(x * y);
		glReadPixels(
			0,
			0,
			x,
			y,
			GL_RED,
			GL_FLOAT,
			m_SoundDataL.data()
		);
		
		m_SoundDataR.resize(x * y);
		glReadPixels(
			0,
			0,
			x,
			y,
			GL_GREEN,
			GL_FLOAT,
			m_SoundDataR.data()
		);
	}
}