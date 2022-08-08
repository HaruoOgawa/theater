#include "GraphicsRenderer.h"

#ifdef _DEBUG
#include "GraphicsEngine/Message/Console.h"
#endif // _DEBUG

#include "../Object/GameObject.h"
#include "../Component/MeshRendererComponent.h"

#include "../GraphicsMain/GraphicsMain.h"
#include "Texture.h"
#include "GraphicsEngine/Graphics/PolygonRaymarchingMixer.h"
#include "GraphicsEngine/Graphics/PostProcess.h"
#include "Assets/App/GenocideCronus/GenocideCronus.h"

GraphicsRenderer* GraphicsRenderer::renderer_instance = nullptr;

void GraphicsRenderer::Create() {
	if (!renderer_instance) {
		renderer_instance = new GraphicsRenderer(GraphicsMain::GetInstance());
	}
}

void GraphicsRenderer::Destroy() {
	delete renderer_instance;
	renderer_instance = nullptr;
}

int GraphicsRenderer::CheckError() {
	int Error = 0;
#ifdef _DEBUG
	// https://qiita.com/_ydah/items/da56763e94ba58af3d91
	GLenum err;
	err = glGetError();

	switch (err)
	{
	case 0: // 0x0000
		Console::Log("ErrorCode %d / GL_NO_ERROR\n", err);
		break;

	case 1280: // 0x0500
		Console::Log("ErrorCode %d / GL_INVALID_ENUM\n", err);
		break;

	case 1281: // 0x0501
		Console::Log("ErrorCode %d / GL_INVALID_VALUE\n", err);
		break;

	case 1282: // 0x0502
		Console::Log("ErrorCode %d / GL_INVALID_OPERATION\n", err);
		break;

	case 1283: // 0x0503
		Console::Log("ErrorCode %d / GL_STACK_OVERFLOW\n", err);
		break;

	case 1284: // 0x0504
		Console::Log("ErrorCode %d / GL_STACK_UNDERFLOW\n", err);
		break;

	case 1285: // 0x0505
		Console::Log("ErrorCode %d / GL_OUT_OF_MEMORY\n", err);
		break;

	case 1286: // 0x0506
		Console::Log("ErrorCode %d / GL_INVALID_FRAMEBUFFER_OPERATION\n", err);
		break;

	case 1287: // 0x0507
		Console::Log("ErrorCode %d / GL_CONTEXT_LOST\n", err);
		break;

	default: // 0x????
		Console::Log("ErrorCode %d / Unknown Error\n", err);
		break;
	}

	Error = static_cast<int>(err);
	//Console::Log("CheckError GLErr: %d\n", err);
#endif // _DEBUG

	return Error;
}

GraphicsRenderer::GraphicsRenderer(GraphicsMain* game)
	: mgame(game),
	sWindow(nullptr),
	sWindowWidth(1920),
	sWindowHeight(1024),
	deltaTime(0.0f),
	frameResolusion(0.7),
	polygon_frameTexture(std::make_shared<Texture>()),
	polygon_depthTexture(std::make_shared<Texture>()),
	raymarching_frameTexture(std::make_shared<Texture>()),
	raymarching_depthTexture(std::make_shared<Texture>()),
	polygon_normalTexture(std::make_shared<Texture>()),
	p_r_BlendingTexture(std::make_shared<Texture>()),
	p_r_DepthBlendingTexture(std::make_shared<Texture>()),
	m_PolygonPostProcess_FrameTexture(std::make_shared<Texture>()),
	m_LatePostProcess_FrameTexture(std::make_shared<Texture>()),
	m_BackgroudColor(glm::vec4(0.0f,0.0f,0.0f,1.0f)),
	polygon_frameBuffer(0),
	polygon_depthBuffer(0),
	polygon_normalBuffer(0),
	raymarching_frameBuffer(0),
	raymarching_depthBuffer(0),
	p_r_BlendingBuffer(0),
	p_r_DepthBlendingBuffer(0),
	m_PolygonPostProcess_FrameBuffer(0),
	m_LatePostProcess_FrameBuffer(0)
{
}

GraphicsRenderer::~GraphicsRenderer() {
	PostProcess::DestroyInstance();
	if (polygon_frameBuffer != 0)glDeleteFramebuffers(1, &polygon_frameBuffer);
	if (polygon_frameBuffer_MSAA != 0)glDeleteFramebuffers(1, &polygon_frameBuffer_MSAA);
	if (polygon_depthBuffer != 0)glDeleteFramebuffers(1, &polygon_depthBuffer);
	if (polygon_normalBuffer != 0)glDeleteFramebuffers(1, &polygon_normalBuffer);
	if (raymarching_frameBuffer != 0)glDeleteFramebuffers(1, &raymarching_frameBuffer);
	if (raymarching_depthBuffer != 0)glDeleteFramebuffers(1, &raymarching_depthBuffer);
	if (p_r_BlendingBuffer != 0)glDeleteFramebuffers(1, &p_r_BlendingBuffer);
	if (p_r_DepthBlendingBuffer != 0)glDeleteFramebuffers(1, &p_r_DepthBlendingBuffer);
	if (m_PolygonPostProcess_FrameBuffer != 0)glDeleteFramebuffers(1, &m_PolygonPostProcess_FrameBuffer);
	if (m_LatePostProcess_FrameBuffer != 0)glDeleteFramebuffers(1, &m_LatePostProcess_FrameBuffer);
}

bool GraphicsRenderer::Initialize(float width,float height) {
	// OpenGLの設定
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
	
	// ウィンドウ生成
	sWindow = glfwCreateWindow(
		sWindowWidth,
		sWindowHeight,
		"Haru86_GraphicsEngine",
		NULL,
		//glfwGetPrimaryMonitor(),
		NULL
	);

	if (!sWindow) {
#ifdef _DEBUG
		Console::Log("Error : glfwCreateWindow\n");
#endif // _DEBUG
		return false;
	}

	// プログラム終了時の処理
	atexit(glfwTerminate);
	
	// 現在のサイズを取得
	int w, h;
	//glfwGetWindowSize(sWindow, &w, &h);
	glfwGetFramebufferSize(sWindow, &w, &h);
	sWindowWidth = static_cast<float>(w);
	sWindowHeight = static_cast<float>(h);

	// コンテキストを作成
	glfwMakeContextCurrent(sWindow);

	// glewの初期化
	glewExperimental = GL_TRUE;
	const GLenum error = glewInit();
	if (error != GLEW_OK) {
#ifdef _DEBUG
		Console::Log("Failure! Can not initialize Glew: %s", glewGetErrorString(error));
#endif // _DEBUG

		return false;
	}
	glGetError();

	// Create PostProcess
	PostProcess::CreateInstance();

	//CreateFrameBuffer
	CreateFrameBuffer(static_cast<int>(GetScreenSize().x), static_cast<int>(GetScreenSize().y),polygon_frameTexture, polygon_frameBuffer, GL_RGBA16F, GL_RGBA, GL_FLOAT);
	CreateFrameBuffer(static_cast<int>(GetScreenSize().x), static_cast<int>(GetScreenSize().y),nullptr, polygon_frameBuffer_MSAA,
		GL_RGBA, GL_RGBA, GL_FLOAT,ERenderTargetType::COLOR_RENDER_BUFFER,EDepthTargetType::DEPTH_RENDER_BUFFER,true);
	
	CreateFrameBuffer(static_cast<int>(GetScreenSize().x), static_cast<int>(GetScreenSize().y), polygon_depthTexture, polygon_depthBuffer, GL_RGBA, GL_RGBA,
		GL_FLOAT,ERenderTargetType::NONECOLORBUFFER,EDepthTargetType::DEPTH_TEXTURE_BUFFER);
	CreateFrameBuffer(static_cast<int>(GetScreenSize().x), static_cast<int>(GetScreenSize().y), raymarching_frameTexture, raymarching_frameBuffer, GL_RGBA, GL_RGBA);
	CreateFrameBuffer(static_cast<int>(GetScreenSize().x), static_cast<int>(GetScreenSize().y), raymarching_depthTexture, raymarching_depthBuffer, GL_RGBA, GL_RGBA);
	CreateFrameBuffer(static_cast<int>(GetScreenSize().x), static_cast<int>(GetScreenSize().y), polygon_normalTexture, polygon_normalBuffer, GL_RGBA, GL_RGBA);
	CreateFrameBuffer(static_cast<int>(GetScreenSize().x), static_cast<int>(GetScreenSize().y), p_r_BlendingTexture, p_r_BlendingBuffer, GL_RGBA16F, GL_RGBA, GL_FLOAT);
	CreateFrameBuffer(static_cast<int>(GetScreenSize().x), static_cast<int>(GetScreenSize().y), p_r_DepthBlendingTexture, p_r_DepthBlendingBuffer, GL_RGBA16F, GL_RGBA, GL_FLOAT);
	CreateFrameBuffer(static_cast<int>(GetScreenSize().x), static_cast<int>(GetScreenSize().y), m_PolygonPostProcess_FrameTexture, m_PolygonPostProcess_FrameBuffer, GL_RGBA16F, GL_RGBA, GL_FLOAT);
	CreateFrameBuffer(static_cast<int>(GetScreenSize().x), static_cast<int>(GetScreenSize().y), m_LatePostProcess_FrameTexture, m_LatePostProcess_FrameBuffer, GL_RGBA16F, GL_RGBA, GL_FLOAT);

	//
	m_Mixer = std::make_unique<PolygonRaymarchingMixer>();

	return true;
}

glm::vec2 GraphicsRenderer::GetScreenSize() {
	return glm::vec2(sWindowWidth, sWindowHeight);
}

GLFWwindow* GraphicsRenderer::GetWindow() {
	return sWindow;
}

// 背景色
void GraphicsRenderer::SetBackgroudColor(glm::vec4 BackgroudColor) {
	m_BackgroudColor = BackgroudColor;
}

bool GraphicsRenderer::CreateFrameBuffer(int width, int height, std::shared_ptr<Texture> fTex, unsigned int& fBuffer,
	GLint internalformat, GLint format, GLenum type, ERenderTargetType RenderTargetType, EDepthTargetType DepthTargetType, bool UseMSAA) {
	
	// フレームバッファ生成
	glGenFramebuffers(1, &fBuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, fBuffer);

	// デプスバッファ生成
	if (DepthTargetType == EDepthTargetType::DEPTH_TEXTURE_BUFFER)
	{
		fTex->CreateForRendering(width, height, GL_DEPTH_COMPONENT16, GL_DEPTH_COMPONENT, GL_FLOAT);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, fTex->GetTextureID(), 0);
	}
	else 
	{
		GLuint depthBuffer;
		glGenRenderbuffers(1, &depthBuffer);
		glBindRenderbuffer(GL_RENDERBUFFER, depthBuffer);

		// アンチエイリアシングを使用
		if (UseMSAA)
		{
			glRenderbufferStorageMultisample(GL_RENDERBUFFER, 16, GL_DEPTH_COMPONENT32F, width, height);
		}
		else
		{
			glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT32F, width, height);
		}
		
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthBuffer);
		glBindRenderbuffer(GL_RENDERBUFFER, 0);
	}
	
	// カラーバッファ生成
	if (RenderTargetType==ERenderTargetType::COLOR_TEXTURE_BUFFER) // カラーテクスチャバッファ
	{ 
		fTex->CreateForRendering(width, height, internalformat, format, type);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, fTex->GetTextureID(), 0);
	}
	else if(RenderTargetType == ERenderTargetType::COLOR_RENDER_BUFFER) // カラーレンダーバッファ
	{
		GLuint colorRenderBuffer;
		glGenRenderbuffers(1, &colorRenderBuffer);
		glBindRenderbuffer(GL_RENDERBUFFER, colorRenderBuffer);

		// アンチエイリアシングを使用
		if (UseMSAA)
		{
			glRenderbufferStorageMultisample(GL_RENDERBUFFER, 16, internalformat, width, height);
		}
		else
		{
			glRenderbufferStorage(GL_RENDERBUFFER, internalformat, width, height);
		}

		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, colorRenderBuffer);
		glBindRenderbuffer(GL_RENDERBUFFER, 0);
	}
	else if (RenderTargetType == ERenderTargetType::REALTIME_CUBEMAP) // 動的キューブマップ
	{
		fTex->CreateForCubemap(width, height, internalformat, format, type);
	}
	else if (RenderTargetType == ERenderTargetType::NONECOLORBUFFER)
	{
		glDrawBuffer(GL_NONE);
		glReadBuffer(GL_NONE);
	}
	else {
		return false;
	}
	
	// バインド解除
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	return true;

}

void GraphicsRenderer::CopyFrameBuffer(unsigned int ReadBuffer, unsigned int DrawBuffer, int width, int height)
{
	glBindFramebuffer(GL_READ_FRAMEBUFFER, ReadBuffer);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, DrawBuffer);

	glBlitFramebuffer(
		0,0, width, height,
		0,0, width, height,
		GL_COLOR_BUFFER_BIT,GL_NEAREST
	);

	glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
}

void GraphicsRenderer::Draw(const std::shared_ptr<TransformComponent>& UsingCamera, bool IsDrawRay,int ResultFrameBufferIndex, std::function<void(void)> callback, int width, int height)
{
	// これからレンダリングするカメラをセット
	GraphicsMain::GetInstance()->m_UsingCamera = UsingCamera;

	//ポリゴンオブジェクトのカラーマップをレンダリング///////////////////
	GraphicsMain::GetInstance()->renderingTarget = ERerderingTarget::COLOR;
	glBindFramebuffer(GL_FRAMEBUFFER, polygon_frameBuffer_MSAA);
	glViewport(0, 0, static_cast<int>(GetScreenSize().x * frameResolusion), static_cast<int>(GetScreenSize().y * frameResolusion));

	glClearColor(m_BackgroudColor.r, m_BackgroudColor.g, m_BackgroudColor.b, m_BackgroudColor.a);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glEnable(GL_DEPTH_TEST);

	for (auto obj : mgame->gameObjectList) {
		obj->m_transform->CalMatrix();
		obj->meshComp->Draw();
	}

	if (mgame->m_App) {
		mgame->m_App->Draw(false);
	}

	// MSAAカラーマップを通常のカラーマップにコピー
	CopyFrameBuffer(polygon_frameBuffer_MSAA, polygon_frameBuffer, polygon_frameTexture->GetWidth(), polygon_frameTexture->GetHeight());

	//ポリゴンオブジェクトをデプスマップをレンダリング
	GraphicsMain::GetInstance()->renderingTarget = ERerderingTarget::COLOR;
	//GraphicsMain::GetInstance()->renderingTarget = ERerderingTarget::DEPTH;
	glBindFramebuffer(GL_FRAMEBUFFER, polygon_depthBuffer);
	glViewport(0, 0, static_cast<int>(GetScreenSize().x * frameResolusion), static_cast<int>(GetScreenSize().y * frameResolusion));

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	//glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glClear(GL_DEPTH_BUFFER_BIT);
	//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glEnable(GL_DEPTH_TEST);

	for (auto obj : mgame->gameObjectList) {
		obj->m_transform->CalMatrix();
		obj->meshComp->Draw();
	}

	if (mgame->m_App) {
		mgame->m_App->Draw(false);
	}

	// ポリゴンオブジェクトのノーマルマップをレンダリング
	GraphicsMain::GetInstance()->renderingTarget = ERerderingTarget::NORMAL;
	glBindFramebuffer(GL_FRAMEBUFFER, polygon_normalBuffer);
	glViewport(0, 0, static_cast<int>(GetScreenSize().x * frameResolusion), static_cast<int>(GetScreenSize().y * frameResolusion));

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glEnable(GL_DEPTH_TEST);

	for (auto obj : mgame->gameObjectList) {
		obj->m_transform->CalMatrix();
		obj->meshComp->Draw();
	}

	if (mgame->m_App) {
		mgame->m_App->Draw(false);
	}


	// ポリゴンオブジェクトのポストプロセス
	PostProcess::GetInstance()->DrawPolygonPostProcess(polygon_frameTexture, m_PolygonPostProcess_FrameBuffer);

	// レイマーチングをレンダリングするのはデフォルトバッファのみ(リフレクションプローブでは無視)
	//レイマーチングオブジェクトのカラーマップをレンダリング///////////////
	if (IsDrawRay) {
		GraphicsMain::GetInstance()->renderingTarget = ERerderingTarget::COLOR;
		glBindFramebuffer(GL_FRAMEBUFFER, raymarching_frameBuffer);
		glViewport(0, 0, static_cast<int>(GetScreenSize().x * frameResolusion), static_cast<int>(GetScreenSize().y * frameResolusion));

		//glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glEnable(GL_DEPTH_TEST);

		for (auto obj : mgame->raymarchingObjectList) {
			obj->m_transform->CalMatrix();
			obj->meshComp->Draw();
		}

		if (mgame->m_App) {
			mgame->m_App->Draw(true);
		}
	}

	//レイマーチングオブジェクトのデプスマップをレンダリング
	if (IsDrawRay) {
		GraphicsMain::GetInstance()->renderingTarget = ERerderingTarget::DEPTH;
		glBindFramebuffer(GL_FRAMEBUFFER, raymarching_depthBuffer);
		glViewport(0, 0, static_cast<int>(GetScreenSize().x * frameResolusion), static_cast<int>(GetScreenSize().y * frameResolusion));

		//glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glEnable(GL_DEPTH_TEST);

		for (auto obj : mgame->raymarchingObjectList) {
			obj->m_transform->CalMatrix();
			obj->meshComp->Draw();
		}

		if (mgame->m_App) {
			mgame->m_App->Draw(true);
		}
	}
	
	//ポリゴンオブジェクトとレイマーチングオブジェクトのカラーバッファをブレンドする
	GraphicsMain::GetInstance()->renderingTarget = ERerderingTarget::COLOR;
	glBindFramebuffer(GL_FRAMEBUFFER, p_r_BlendingBuffer);
	glViewport(0, 0, static_cast<int>(GetScreenSize().x * frameResolusion), static_cast<int>(GetScreenSize().y * frameResolusion));

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glEnable(GL_DEPTH_TEST);

	// ミックス
	m_Mixer->Draw(false);
	
	//ポリゴンオブジェクトとレイマーチングオブジェクトのデプスバッファをブレンドする
	GraphicsMain::GetInstance()->renderingTarget = ERerderingTarget::COLOR;
	glBindFramebuffer(GL_FRAMEBUFFER, p_r_DepthBlendingBuffer);
	glViewport(0, 0, static_cast<int>(GetScreenSize().x * frameResolusion), static_cast<int>(GetScreenSize().y * frameResolusion));

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glEnable(GL_DEPTH_TEST);

	// ミックス
	m_Mixer->Draw(true);


	//ミキシングしたフレームバッファのポストプロセス////////////////////////////////////////
	PostProcess::GetInstance()->DrawLatePostProcess(p_r_BlendingTexture, m_LatePostProcess_FrameBuffer);

	//最終的な結果を作成する///////////////////////////////
	GraphicsMain::GetInstance()->renderingTarget = ERerderingTarget::COLOR;
	glBindFramebuffer(GL_FRAMEBUFFER, ResultFrameBufferIndex);
	
	// 最終結果前に何かしたいならコールバックを呼び出す
	callback();

	glViewport(0, 0, width, height);

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	glEnable(GL_DEPTH_TEST);
	glDisable(GL_BLEND);

	for (auto obj : mgame->boardGameObjectList) {
		obj->meshComp->Draw();
	}
}