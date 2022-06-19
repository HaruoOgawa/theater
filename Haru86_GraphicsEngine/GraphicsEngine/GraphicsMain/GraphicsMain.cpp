#include "GraphicsMain.h"
#include <string>
#include "../Object/GameObject.h"
#include "../Graphics/Mesh.h"
#include "../Component/AudioSourceComponent.h"
#include "../Object/TimelineObject.h"
#include "../Component/TimelineComponent.h"
#include "./Time.h"
#include <iostream>
#include <vector>
#include <algorithm>
#include "../App/BaseApp/BaseApp.h"
#include <exception>
#include <stdexcept>
#include "GraphicsEngine/Object/RaymarchingObject.h"
#include "GraphicsEngine/Object/CameraObject.h"
#include "GraphicsEngine/Message/Console.h"
#include "GraphicsEngine/App/CEventListener.h"
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
	isRestart(false),
	renderingTarget(ERerderingTarget::COLOR),
	m_RaymarchingObject(nullptr),
	m_EventListener(std::make_shared<app::CEventListener>())
{
	for (int i = 0; i < 20;i++) {
		switch (i)
		{
		case 0:
			texSlots.emplace(i, GL_TEXTURE0);
			break;
		case 1:
			texSlots.emplace(i, GL_TEXTURE1);
			break;
		case 2:
			texSlots.emplace(i, GL_TEXTURE2);
			break;
		case 3:
			texSlots.emplace(i, GL_TEXTURE3);
			break;
		case 4:
			texSlots.emplace(i, GL_TEXTURE4);
			break;
		case 5:
			texSlots.emplace(i, GL_TEXTURE5);
			break;
		case 6:
			texSlots.emplace(i, GL_TEXTURE6);
			break;
		case 7:
			texSlots.emplace(i, GL_TEXTURE7);
			break;
		case 8:
			texSlots.emplace(i, GL_TEXTURE8);
			break;
		case 9:
			texSlots.emplace(i, GL_TEXTURE9);
			break;
		case 10:
			texSlots.emplace(i, GL_TEXTURE10);
			break;
		case 11:
			texSlots.emplace(i, GL_TEXTURE11);
			break;
		case 12:
			texSlots.emplace(i, GL_TEXTURE12);
			break;
		case 13:
			texSlots.emplace(i, GL_TEXTURE13);
			break;
		case 14:
			texSlots.emplace(i, GL_TEXTURE14);
			break;
		case 15:
			texSlots.emplace(i, GL_TEXTURE15);
			break;
		case 16:
			texSlots.emplace(i, GL_TEXTURE16);
			break;
		case 17:
			texSlots.emplace(i, GL_TEXTURE17);
			break;
		case 18:
			texSlots.emplace(i, GL_TEXTURE18);
			break;
		case 19:
			texSlots.emplace(i, GL_TEXTURE19);
			break;
		default:
			break;
		}
	}
}

GraphicsMain::~GraphicsMain() {
	
}

bool GraphicsMain::CreateApp() {
	if (glfwInit() == GL_FALSE) {
		Console::Log("Could not initialize GLFW\n");
		return false;
	}

	GraphicsRenderer::Create();
	if (!GraphicsRenderer::GetInstance()->Initialize(500, 500)) {
		Console::Log("Could not Create GraphicsRenderer\n");
		return false;
	}

	return true;
}

bool GraphicsMain::Initialize(BaseApp* app) {
	// メモリ確保
	timeObj = std::make_unique<Time>(60.0f);
	timelineObj = std::make_unique<TimelineObject>();
	m_App = app;

	// 初期化が必要なパラメーターを初期化
	isRestart = false;
	
	//
	LoadData();
	
	return true;
}

void GraphicsMain::LoadData() {
	//
	m_App->Start();

	//renderBoardがユーザーに指定されていないのであれば、デフォルトのものをセットする
	if (renderBoard==nullptr) {
		renderBoard = std::make_unique<GameObject>(PrimitiveType::BOARD, RenderType::FrameBuffer,RenderQueue::UI,
			shaderlib::ShaderLib::StandardRenderBoard_vert, shaderlib::ShaderLib::StandardRenderBoard_frag);
	}

	if (!game_camera_instance) {
		game_camera_instance= std::make_shared<CameraObject>(CameraType::FIXED_CAMERA);
		game_camera_instance->SetPosition(glm::vec3(0.0f,0.0f,3.0f));
	}

	//
	m_App->Timeline(timelineObj.get());
	timelineObj->Initialize();

}

bool GraphicsMain::RunLoop() {
	while (isRunning)
	{
		if (isRestart) {
			if (Reflesh())return true;
		}
		else {
			UpdateTimeline();
			InputProcess();
			Update();
			Draw();
		}
	}

	return false;
}

void GraphicsMain::UpdateTimeline() {
	static_cast<std::shared_ptr<class TimelineComponent>>(timelineObj->timelineComponent)->Update();

	std::sort(gameObjectList.begin(), gameObjectList.end(), [](GameObject* a, GameObject* b) {
		return a->m_renderOrder < b->m_renderOrder;
	});
}

void GraphicsMain::InputProcess() {
	// イベント
	m_EventListener->ListenEvent(GraphicsRenderer::GetInstance()->GetWindow());
	
	for (auto obj : gameObjectList) {
		obj->ProcessInput(m_EventListener);
	}

	game_camera_instance->ProcessInput(m_EventListener);

}

void GraphicsMain::Update() {
	while (!(timeObj->GetTime() > previousTime + 16.0f)) { timeObj->UpdateTime(); };
	deltaTime = (timeObj->GetTime() - previousTime) / 1000.0f;
	if (deltaTime > 0.05f) {
		deltaTime = 0.05f;
	}
	previousTime = static_cast<float>(timeObj->GetTime());
	time = previousTime;

	for (auto obj : gameObjectList) {
		obj->Update();
	}
	
	if (m_App) {
		m_App->Update();
	}

	game_camera_instance->Update();
	
}
void GraphicsMain::Draw() {
	GraphicsRenderer::GetInstance()->Draw();
}

bool GraphicsMain::Reflesh() {
	//Listを解放
	gameObjectList.clear();
	boardGameObjectList.clear();
	postProcessGameObjectList.clear();
	uiObjectList.clear();

	//メンバ変数を初期化
	isRunning = true;
	time = 0.0f;
	deltaTime = 0.0f;
	previousTime = 0.0f;
	mouseStateBool = false;
	animTime = 0.0f;
	renderingTarget = ERerderingTarget::COLOR;

	//メモリを解放
	game_camera_instance = nullptr;
	timelineObj.reset();
	timeObj.reset();
	renderBoard.reset();
	if (m_App) {
		delete m_App;
		m_App = nullptr; 
	}
	m_RaymarchingObject = nullptr;
	
	return true;
}

void GraphicsMain::Restart() {
	isRestart = true;
}

void GraphicsMain::ShutDown() {
	GraphicsRenderer::GetInstance()->ShutDown();
	GraphicsRenderer::Destroy();

	gameObjectList.clear();
	boardGameObjectList.clear();
	postProcessGameObjectList.clear();
	uiObjectList.clear();
}

void GraphicsMain::SetIsRunning(bool state) {
	isRunning = state;
}