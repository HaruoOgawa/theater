#include "GraphicsMain.h"
#include <string>
#include "../Object/GameObject.h"
#include "../Graphics/Mesh.h"
#include "CTimeline.h"
#include <vector>
#include <algorithm>
#include "Assets/App/GenocideCronus/GenocideCronus.h"
#include "GraphicsEngine/Sound/SoundShaderPlayer.h"

#ifdef _DEBUG
#include "GraphicsEngine/Message/Console.h"
#endif // _DEBUG

#include "GraphicsEngine/Graphics/ShaderLib.h"

GraphicsMain* GraphicsMain::s_pInstance = nullptr;

void GraphicsMain::Create()
{
	if (!s_pInstance)
	{
		s_pInstance = new GraphicsMain;
	}
}

void GraphicsMain::Destroy()
{
	delete s_pInstance;
	s_pInstance = nullptr;
}

GraphicsMain::GraphicsMain()
	: 
	isRunning(true),
	time(0.0f),
	deltaTime(0.0f),
	previousTime(0.0f),
	mouseStateBool(false),
	animTime(0.0f),
	renderingTarget(ERerderingTarget::COLOR),
	m_timeline(nullptr),
	m_CameraTransform(nullptr)
{
}

GraphicsMain::~GraphicsMain() {
	GraphicsRenderer::Destroy();

	gameObjectList.clear();
	raymarchingObjectList.clear();
	boardGameObjectList.clear();
	postProcessGameObjectList.clear();
	uiObjectList.clear();
}

bool GraphicsMain::CreateApp() {
	if (glfwInit() == GL_FALSE) {
		//Console::Log("Could not initialize GLFW\n");
		return false;
	}

	GraphicsRenderer::Create();
	if (!GraphicsRenderer::GetInstance()->Initialize(500, 500)) {
		//Console::Log("Could not Create GraphicsRenderer\n");
		return false;
	}

	return true;
}

bool GraphicsMain::Initialize() {
	// メモリ確保
	m_timeline = std::make_unique<CTimeline>();
	m_App = new GenocideCronus();
	LoadData();
	
	return true;
}

void GraphicsMain::LoadData() {
#ifdef _DEBUG
	Console::Log("This is _DEBUG!!!!!!!!!!!!!!!!!!!!!\n");
#endif // DEBUG
	
	//
	m_App->Start();

	//renderBoardがユーザーに指定されていないのであれば、デフォルトのものをセットする
	if (renderBoard==nullptr) {
		renderBoard = std::make_unique<GameObject>(PrimitiveType::BOARD, RenderType::FrameBuffer,RenderQueue::UI,RenderingSurfaceType::RASTERIZER,
			shaderlib::ShaderLib::StandardRenderBoard_vert, shaderlib::ShaderLib::StandardRenderBoard_frag);
	}

	//
	m_App->Timeline(m_timeline.get());
	m_timeline->Initialize();
}

bool GraphicsMain::RunLoop() {
	while (isRunning)
	{
		UpdateTimeline();
		InputProcess();
		Update();
		Draw();
	}

	return false;
}

void GraphicsMain::UpdateTimeline() {
	m_timeline->Update();
	std::sort(gameObjectList.begin(), gameObjectList.end(), [](GameObject* a, GameObject* b) {
		return a->m_renderOrder < b->m_renderOrder;
	});
}

void GraphicsMain::InputProcess() {
	// イベントを発行
	glfwPollEvents();
	glfwSetKeyCallback(GraphicsRenderer::GetInstance()->GetWindow(), key_callback);
}

void GraphicsMain::key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
		GraphicsMain::GetInstance()->isRunning=false;
	}
}

void GraphicsMain::Update() {
	while (!(time > previousTime + 16.0f)) { time += (1.0f / 60.0f); };
	deltaTime = (time - previousTime) / 1000.0f;
	if (deltaTime > 0.05f) {
		deltaTime = 0.05f;
	}
	previousTime = time;

	if (m_App) {
		m_App->Update();
	}

}
void GraphicsMain::Draw() {
	GraphicsRenderer::GetInstance()->Draw();
}
