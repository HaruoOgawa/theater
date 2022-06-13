#include "GraphicsRenderer.h"
#include "GL/glew.h"
#include "../Object/GameObject.h"
#include "../Component/MeshRendererComponent.h"
#include "../Component/TransformComponent.h"
#include "../GraphicsMain/GraphicsMain.h"
#include "./Texture.h"
#include "GraphicsEngine/App/BaseApp/BaseApp.h"
#include "GraphicsEngine/Object/RaymarchingObject.h"
#include "GraphicsEngine/Graphics/PolygonRaymarchingMixer.h"
#include "GraphicsEngine/Graphics/PostProcess.h"
#include "GraphicsEngine/Object/CNode.h"

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
	sWindowWidth(500),
	sWindowHeight(500),
	deltaTime(0.0f),
	frameResolusion(0.7),
	polygon_frameTexture(std::make_shared<Texture>()),
	polygon_depthTexture(std::make_shared<Texture>()),
	raymarching_frameTexture(std::make_shared<Texture>()),
	raymarching_depthTexture(std::make_shared<Texture>()),
	p_r_BlendingTexture(std::make_shared<Texture>()),
	m_PolygonPostProcess_FrameTexture(std::make_shared<Texture>()),
	m_LatePostProcess_FrameTexture(std::make_shared<Texture>()),
	m_BackgroudColor(glm::vec4(0.0f,0.0f,0.0f,1.0f))
{
}

GraphicsRenderer::~GraphicsRenderer() {

}

bool GraphicsRenderer::Initialize(float width,float height) {
	
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_RED_SIZE,8);
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE,8);
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE,8);
	SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE,8);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE,24);
	SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL,1);

	sWindow = SDL_CreateWindow(
		"Haru86_GLVJ",
		100,
		100,
		static_cast<int>(sWindowWidth),
		static_cast<int>(sWindowHeight),
		SDL_WINDOW_OPENGL
	);

	if (!sWindow) {
		//SDL_Log("Failure! Can not initialize SDL_OpenGL Window: %s", SDL_GetError());
		return false;
	}
	
	if (SDL_SetWindowFullscreen(sWindow, SDL_WINDOW_FULLSCREEN_DESKTOP)!=0) {
		//SDL_Log("Full Screeen Error: %s", SDL_GetError());
	}

	SDL_GetWindowSize(sWindow, &sWindowWidth, &sWindowHeight);
	
	context = SDL_GL_CreateContext(sWindow);

	glewExperimental = GL_TRUE;
	const GLenum error = glewInit();
	if (error != GLEW_OK) {
		//SDL_Log("Failure! Can not initialize Glew: %s",glewGetErrorString(error));
		return false;
	}
	glGetError();

	// Create PostProcess
	PostProcess::CreateInstance();

	//CreateFrameBuffer
	if (!CreateFrameBuffer(polygon_frameTexture, polygon_frameBuffer, GL_RGBA16F, GL_RGBA, GL_FLOAT)) {
		//SDL_Log("Can not create frame buffer");
	}
	
	if (!CreateFrameBuffer(polygon_depthTexture, polygon_depthBuffer,GL_RGBA, GL_RGBA)) {
		//SDL_Log("Can not create frame buffer");
	}

	if (!CreateFrameBuffer(raymarching_frameTexture, raymarching_frameBuffer, GL_RGBA, GL_RGBA)) {
		//SDL_Log("Can not create frame buffer");
	}

	if (!CreateFrameBuffer(raymarching_depthTexture, raymarching_depthBuffer, GL_RGBA, GL_RGBA)) {
		//SDL_Log("Can not create frame buffer");
	}
	
	if (!CreateFrameBuffer(p_r_BlendingTexture, p_r_BlendingBuffer, GL_RGBA16F, GL_RGBA, GL_FLOAT)) {
		//SDL_Log("Can not create frame buffer");
	}
	
	if (!CreateFrameBuffer(m_PolygonPostProcess_FrameTexture, m_PolygonPostProcess_FrameBuffer, GL_RGBA16F, GL_RGBA, GL_FLOAT)) {
		//SDL_Log("Can not create frame buffer");
	}
	
	if (!CreateFrameBuffer(m_LatePostProcess_FrameTexture, m_LatePostProcess_FrameBuffer, GL_RGBA16F, GL_RGBA, GL_FLOAT)) {
		//SDL_Log("Can not create frame buffer");
	}

	m_Mixer = std::make_unique<PolygonRaymarchingMixer>();

	return true;
}

glm::ivec2 GraphicsRenderer::GetScreenSize() {
	return glm::ivec2(sWindowWidth, sWindowHeight);
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
	//ポリゴンオブジェクトのカラーマップをレンダリング///////////////////
	GraphicsMain::GetInstance()->renderingTarget = ERerderingTarget::COLOR;
	glBindFramebuffer(GL_FRAMEBUFFER, polygon_frameBuffer);
	glViewport(0, 0, static_cast<int>(GetScreenSize().x * frameResolusion), static_cast<int>(GetScreenSize().y * frameResolusion));

	glClearColor(m_BackgroudColor.r, m_BackgroudColor.g, m_BackgroudColor.b, m_BackgroudColor.a);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glEnable(GL_DEPTH_TEST);

	for (auto obj : mgame->gameObjectList) {
		std::vector<glm::mat4> ModelMatrixTree;
		obj->GetRootNode()->Draw(ModelMatrixTree);
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
		std::vector<glm::mat4> ModelMatrixTree;
		obj->GetRootNode()->Draw(ModelMatrixTree);
		ModelMatrixTree.clear();
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

		GraphicsMain::GetInstance()->m_RaymarchingObject->GetRootNode()->Draw();
	}

	//レイマーチングオブジェクトのデプスマップをレンダリング
	if (GraphicsMain::GetInstance()->m_RaymarchingObject) {
		GraphicsMain::GetInstance()->renderingTarget = ERerderingTarget::DEPTH;
		glBindFramebuffer(GL_FRAMEBUFFER, raymarching_depthBuffer);
		glViewport(0, 0, static_cast<int>(GetScreenSize().x * frameResolusion), static_cast<int>(GetScreenSize().y * frameResolusion));

		glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glEnable(GL_DEPTH_TEST);

		GraphicsMain::GetInstance()->m_RaymarchingObject->GetRootNode()->Draw();
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

	//垂直同期を実行
	SDL_GL_SwapWindow(sWindow);
}

void GraphicsRenderer::ShutDown() {
	SDL_GL_DeleteContext(context);
	SDL_DestroyWindow(sWindow);

	PostProcess::DestroyInstance();
	
	if (polygon_frameTexture!=nullptr) {
		glDeleteFramebuffers(1, &polygon_frameBuffer);
		polygon_frameTexture->Unload();
		polygon_frameTexture.reset();
		polygon_frameTexture = nullptr;
	}

	if (polygon_depthTexture != nullptr) {
		glDeleteFramebuffers(1, &polygon_depthBuffer);
		polygon_depthTexture->Unload();
		polygon_depthTexture.reset();
		polygon_depthTexture = nullptr;
	}

	if (raymarching_frameTexture != nullptr) {
		glDeleteFramebuffers(1, &raymarching_frameBuffer);
		raymarching_frameTexture->Unload();
		raymarching_frameTexture.reset();
		raymarching_frameTexture = nullptr;
	}

	if (raymarching_depthTexture != nullptr) {
		glDeleteFramebuffers(1, &raymarching_depthBuffer);
		raymarching_depthTexture->Unload();
		raymarching_depthTexture.reset();
		raymarching_depthTexture = nullptr;
	}
}
