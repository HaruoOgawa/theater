#include "GraphicsRenderer.h"

#ifdef _DEBUG
#include "GraphicsEngine/Message/Console.h"
#endif // _DEBUG

#include "../Object/GameObject.h"
#include "../Component/MeshRendererComponent.h"
#include "../Component/TransformComponent.h"
#include "../GraphicsMain/GraphicsMain.h"
#include "./Texture.h"
#include "GraphicsEngine/Object/RaymarchingObject.h"
#include "GraphicsEngine/Graphics/PolygonRaymarchingMixer.h"
#include "GraphicsEngine/Graphics/PostProcess.h"
#include "Assets/App/TheaterDemo/TheaterDemo.h"

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
	p_r_BlendingTexture(std::make_shared<Texture>()),
	m_PolygonPostProcess_FrameTexture(std::make_shared<Texture>()),
	m_LatePostProcess_FrameTexture(std::make_shared<Texture>()),
	m_BackgroudColor(glm::vec4(0.0f,0.0f,0.0f,1.0f)),
	polygon_frameBuffer(0),
	polygon_depthBuffer(0),
	raymarching_frameBuffer(0),
	raymarching_depthBuffer(0),
	p_r_BlendingBuffer(0),
	m_PolygonPostProcess_FrameBuffer(0),
	m_LatePostProcess_FrameBuffer(0)
{
}

GraphicsRenderer::~GraphicsRenderer() {

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
	glfwGetWindowSize(sWindow, &sWindowWidth, &sWindowHeight);
	
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
	if (!CreateFrameBuffer(polygon_frameTexture, polygon_frameBuffer, GL_RGBA16F, GL_RGBA, GL_FLOAT)) {
		//Console::Log("Can not create frame buffer");
	}
	
	if (!CreateFrameBuffer(polygon_depthTexture, polygon_depthBuffer,GL_RGBA, GL_RGBA)) {
		//Console::Log("Can not create frame buffer");
	}

	if (!CreateFrameBuffer(raymarching_frameTexture, raymarching_frameBuffer, GL_RGBA, GL_RGBA)) {
		//Console::Log("Can not create frame buffer");
	}

	if (!CreateFrameBuffer(raymarching_depthTexture, raymarching_depthBuffer, GL_RGBA, GL_RGBA)) {
		//Console::Log("Can not create frame buffer");
	}
	
	if (!CreateFrameBuffer(p_r_BlendingTexture, p_r_BlendingBuffer, GL_RGBA16F, GL_RGBA, GL_FLOAT)) {
		//Console::Log("Can not create frame buffer");
	}
	
	if (!CreateFrameBuffer(m_PolygonPostProcess_FrameTexture, m_PolygonPostProcess_FrameBuffer, GL_RGBA16F, GL_RGBA, GL_FLOAT)) {
		//Console::Log("Can not create frame buffer");
	}
	
	if (!CreateFrameBuffer(m_LatePostProcess_FrameTexture, m_LatePostProcess_FrameBuffer, GL_RGBA16F, GL_RGBA, GL_FLOAT)) {
		//Console::Log("Can not create frame buffer");
	}

	m_Mixer = std::make_unique<PolygonRaymarchingMixer>();

	return true;
}

glm::ivec2 GraphicsRenderer::GetScreenSize() {
	return glm::ivec2(sWindowWidth, sWindowHeight);
}

GLFWwindow* GraphicsRenderer::GetWindow() {
	return sWindow;
}

// 背景色
void GraphicsRenderer::SetBackgroudColor(glm::vec4 BackgroudColor) {
	m_BackgroudColor = BackgroudColor;
}

bool GraphicsRenderer::CreateFrameBuffer(std::shared_ptr<Texture> fTex, unsigned int& fBuffer, GLint internalformat, GLint format, GLenum type) {
	int width = static_cast<int>(GetScreenSize().x) ;
	int height = static_cast<int>(GetScreenSize().y );
	
	glGenFramebuffers(1, &fBuffer);
	GLuint depthBuffer;
	glGenRenderbuffers(1, &depthBuffer);

	fTex->CreateForRendering(width, height,internalformat, format, type);
	
	glBindRenderbuffer(GL_RENDERBUFFER, depthBuffer);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT16, width, height);

	glBindFramebuffer(GL_FRAMEBUFFER, fBuffer);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, fTex->GetTextureID(), 0);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthBuffer);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	return true;

}

void GraphicsRenderer::Draw() {
	// 垂直同期の待機時間
	glfwSwapInterval(1);

	//ポリゴンオブジェクトのカラーマップをレンダリング///////////////////
	GraphicsMain::GetInstance()->renderingTarget = ERerderingTarget::COLOR;
	glBindFramebuffer(GL_FRAMEBUFFER, polygon_frameBuffer);
	glViewport(0, 0, static_cast<int>(GetScreenSize().x * frameResolusion), static_cast<int>(GetScreenSize().y * frameResolusion));

	glClearColor(m_BackgroudColor.r, m_BackgroudColor.g, m_BackgroudColor.b, m_BackgroudColor.a);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glEnable(GL_DEPTH_TEST);

	for (auto obj : mgame->gameObjectList) {
		obj->m_transform->CalMatrix();
		obj->meshComp->Draw();
	}

	if (mgame->m_App) {
		mgame->m_App->Draw();
	}

	//ポリゴンオブジェクトをデプスマップをレンダリング
	GraphicsMain::GetInstance()->renderingTarget = ERerderingTarget::DEPTH;
	glBindFramebuffer(GL_FRAMEBUFFER, polygon_depthBuffer);
	glViewport(0, 0, static_cast<int>(GetScreenSize().x * frameResolusion), static_cast<int>(GetScreenSize().y * frameResolusion));

	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glEnable(GL_DEPTH_TEST);

	for (auto obj : mgame->gameObjectList) {
		obj->m_transform->CalMatrix();
		obj->meshComp->Draw();
	}

	if (mgame->m_App) {
		mgame->m_App->Draw();
	}

	// ポリゴンオブジェクトのポストプロセス
	PostProcess::GetInstance()->DrawPolygonPostProcess(polygon_frameTexture, m_PolygonPostProcess_FrameBuffer);

	//レイマーチングオブジェクトのカラーマップをレンダリング///////////////
	if (GraphicsMain::GetInstance()->m_RaymarchingObject) {
		GraphicsMain::GetInstance()->renderingTarget = ERerderingTarget::COLOR;
		glBindFramebuffer(GL_FRAMEBUFFER, raymarching_frameBuffer);
		glViewport(0, 0, static_cast<int>(GetScreenSize().x * frameResolusion), static_cast<int>(GetScreenSize().y * frameResolusion));

		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glEnable(GL_DEPTH_TEST);

		GraphicsMain::GetInstance()->m_RaymarchingObject->m_transform->CalMatrix();
		GraphicsMain::GetInstance()->m_RaymarchingObject->Draw();
	}

	//レイマーチングオブジェクトのデプスマップをレンダリング
	if (GraphicsMain::GetInstance()->m_RaymarchingObject) {
		GraphicsMain::GetInstance()->renderingTarget = ERerderingTarget::DEPTH;
		glBindFramebuffer(GL_FRAMEBUFFER, raymarching_depthBuffer);
		glViewport(0, 0, static_cast<int>(GetScreenSize().x * frameResolusion), static_cast<int>(GetScreenSize().y * frameResolusion));

		glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glEnable(GL_DEPTH_TEST);

		GraphicsMain::GetInstance()->m_RaymarchingObject->m_transform->CalMatrix();
		GraphicsMain::GetInstance()->m_RaymarchingObject->Draw();
	}

	//ポリゴンオブジェクトとレイマーチングオブジェクトをブレンドする
	GraphicsMain::GetInstance()->renderingTarget = ERerderingTarget::COLOR;
	glBindFramebuffer(GL_FRAMEBUFFER, p_r_BlendingBuffer);
	glViewport(0, 0, static_cast<int>(GetScreenSize().x * frameResolusion), static_cast<int>(GetScreenSize().y * frameResolusion));

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glEnable(GL_DEPTH_TEST);

	//ミキシング
	m_Mixer->Draw();

	//ミキシングしたフレームバッファのポストプロセス////////////////////////////////////////
	PostProcess::GetInstance()->DrawLatePostProcess(p_r_BlendingTexture, m_LatePostProcess_FrameBuffer);

	//最終的な結果を作成する///////////////////////////////
	GraphicsMain::GetInstance()->renderingTarget = ERerderingTarget::COLOR;
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glViewport(0, 0, GetScreenSize().x, GetScreenSize().y);

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glEnable(GL_DEPTH_TEST);
	glDisable(GL_BLEND);

	for (auto obj : mgame->boardGameObjectList) {
		obj->meshComp->DrawBoard();
	}

	//カラーバッファを入れ替える
	glfwSwapBuffers(sWindow);

}

void GraphicsRenderer::ShutDown() {
	PostProcess::DestroyInstance();
	
	if (polygon_frameTexture) {
		if(polygon_frameBuffer!=0)glDeleteFramebuffers(1, &polygon_frameBuffer);
		polygon_frameTexture->Unload();
		polygon_frameTexture.reset();
		polygon_frameTexture = nullptr;
	}

	if (polygon_depthTexture) {
		if (polygon_depthBuffer != 0)glDeleteFramebuffers(1, &polygon_depthBuffer);
		polygon_depthTexture->Unload();
		polygon_depthTexture.reset();
		polygon_depthTexture = nullptr;
	}

	if (raymarching_frameTexture) {
		if (raymarching_frameBuffer != 0)glDeleteFramebuffers(1, &raymarching_frameBuffer);
		raymarching_frameTexture->Unload();
		raymarching_frameTexture.reset();
		raymarching_frameTexture = nullptr;
	}

	if (raymarching_depthTexture) {
		if (raymarching_depthBuffer != 0)glDeleteFramebuffers(1, &raymarching_depthBuffer);
		raymarching_depthTexture->Unload();
		raymarching_depthTexture.reset();
		raymarching_depthTexture = nullptr;
	}
}
