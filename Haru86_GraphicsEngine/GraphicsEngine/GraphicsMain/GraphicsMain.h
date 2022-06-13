#pragma once

#include "../Graphics/GraphicsRenderer.h"
#include <vector>

#include <memory>
#include <map>
#include <glfw3.h>
#include <glew.h>

namespace physics { class CPhysicsEngine; }

enum ERerderingTarget
{
	COLOR,
	DEPTH
};

class BaseApp;

namespace app {
	class CEventListener;
}

class GraphicsMain
{
	bool isRunning;
	float previousTime;
	std::vector<class GameObject*> gameObjectList;
	std::vector<class GameObject*> boardGameObjectList;
	std::vector<class GameObject*> postProcessGameObjectList;
	std::vector<class UIObject*> uiObjectList;

	bool mouseStateBool;
	
	ERerderingTarget renderingTarget;

	//
	std::shared_ptr<app::CEventListener> m_EventListener;

public:
	static GraphicsMain* GetInstance()
	{
		return s_pInstance;
	}

	static void Create();
	static void Destroy();

	GraphicsMain();
	~GraphicsMain();
	bool CreateApp();
	bool Initialize(BaseApp* app);
	bool RunLoop();
	void ShutDown();
	void Restart();
	physics::CPhysicsEngine* GetPhysicsEngine();

	float time;
	float deltaTime;
	std::unique_ptr<class Time> timeObj;
	std::unique_ptr<class TimelineObject> timelineObj;
	float animTime;

	//frame board
	std::unique_ptr<class GameObject> renderBoard;
	std::shared_ptr<class CameraObject> game_camera_instance;
	std::map<int, GLenum> texSlots;
	class RaymarchingObject* m_RaymarchingObject;
	
	//
	BaseApp* m_App;

	// ï®óùÉGÉìÉWÉì
	std::unique_ptr<physics::CPhysicsEngine> m_PhysicsEngine;

	//
	bool isRestart;

	

	friend class Main;
	friend class GameObject;
	friend class RaymarchingObject;
	friend class UIObject;
	friend class GraphicsRenderer;
	friend class TimelineComponent;
	friend class UIComponent;
	friend class Material;
	friend class PostProcess;
	friend class CBloom;

private :
	void UpdateTimeline();
	void InputProcess();
	void Update();
	void Draw();
	void LoadData();
	bool Reflesh();
protected:
	static GraphicsMain* s_pInstance;
};

