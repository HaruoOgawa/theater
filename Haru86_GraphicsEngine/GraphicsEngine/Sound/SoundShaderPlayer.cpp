#include "SoundShaderPlayer.h"
#include "GraphicsEngine/Graphics/Mesh.h"
#include "GraphicsEngine/Graphics/Material.h"
#include "GraphicsEngine/Graphics/Texture.h"
#include "GraphicsEngine/Graphics/ShaderLib.h"
#include "GraphicsEngine/Graphics/GraphicsRenderer.h"
#include "GraphicsEngine/GraphicsMain/GraphicsMain.h"

#include <Windows.h>
#include <mmsystem.h>
#include <mmreg.h>

#ifdef _DEBUG
#include "GraphicsEngine/Message/Console.h"
#endif // _DEBUG

namespace sound {
	SoundShaderPlayer::SoundShaderPlayer(const std::string& soundCode):
		m_FrameTex(nullptr),
		m_FrameIndex(0),
		m_SoundLength(180.0f)
	{
		m_Mesh = std::make_shared<Mesh>(PrimitiveType::BOARD);
		m_Material = std::make_shared<Material>(RenderingSurfaceType::RASTERIZER, shaderlib::ShaderLib::StandardRenderBoard_vert, soundCode);
		m_FrameTex = std::make_shared<Texture>();
		GraphicsRenderer::GetInstance()->CreateFrameBuffer(GraphicsRenderer::GetInstance()->GetScreenSize().x, GraphicsRenderer::GetInstance()->GetScreenSize().y, m_FrameTex, m_FrameIndex, GL_RGBA16F, GL_RGBA, GL_FLOAT);
	}

	bool SoundShaderPlayer::Initialize() {
		// SoundShaderの読み込み
		DrawSound();

		// mmeapi sound apiの準備
		//PlaySoundShader();

		return true;
	}

	bool SoundShaderPlayer::Update() {
#ifdef _DEBUG
		//Console::Log("m_SoundData=> [0]:%f, [1]:%f, [2]:%f, [3]:%f\n",
		//	static_cast<float>(m_SoundData[0]), static_cast<float>(m_SoundData[1]), static_cast<float>(m_SoundData[2]), static_cast<float>(m_SoundData[3]));
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
		
		m_Material->SetVec2Uniform("_resolution", GraphicsRenderer::GetInstance()->GetScreenSize());
		m_Material->SetFloatUniform("_frameResolusion", GraphicsRenderer::GetInstance()->frameResolusion);
		m_Material->SetFloatUniform("_soundLength", m_SoundLength); //

		m_Mesh->Draw();

		ReadSoundBuffer();
	}

	void SoundShaderPlayer::ReadSoundBuffer() {
		int x = static_cast<int>(GraphicsRenderer::GetInstance()->GetScreenSize().x * GraphicsRenderer::GetInstance()->frameResolusion);
		int y = static_cast<int>(GraphicsRenderer::GetInstance()->GetScreenSize().y * GraphicsRenderer::GetInstance()->frameResolusion);

		m_SoundData.resize(x * y * 2);
		glReadPixels(
			0,
			0,
			x,
			y,
			GL_RG,
			GL_FLOAT,
			m_SoundData.data()
		);

		//GraphicsRenderer::CheckError();
	}

#define SAMPLE_RATE 44100
#define SAMPLE_TYPE float

	// mmeapi sound apiによる音楽の再生
	void SoundShaderPlayer::PlaySoundShader() {
		// サウンドマネージャー
		HWAVEOUT hWaveOut;
		unsigned int MAX_SAMPLES = static_cast<unsigned int>(m_SoundData.size());
		unsigned int SamplesPerSec = static_cast<unsigned int>(static_cast<float>(MAX_SAMPLES) / m_SoundLength);

		// サウンドの設定
		WAVEFORMATEX WaveFMT = {
			WAVE_FORMAT_IEEE_FLOAT,
			2,
			MAX_SAMPLES * sizeof(SAMPLE_TYPE),
			4,
			SamplesPerSec,
			SamplesPerSec * 4,
			0
		};
			
		// サウンドデータ
		WAVEHDR WaveHDR = {
			(LPSTR)&m_SoundData[0],
			MAX_SAMPLES * sizeof(SAMPLE_TYPE),
			0,
			0,
			0,
			0,
			0,
			0
		};

		//
		waveOutOpen(&hWaveOut, WAVE_MAPPER, &WaveFMT, NULL, 0, CALLBACK_NULL);
		waveOutPrepareHeader(hWaveOut, &WaveHDR, sizeof(WaveHDR));
		waveOutWrite(hWaveOut, &WaveHDR, sizeof(WaveHDR));
	}
}