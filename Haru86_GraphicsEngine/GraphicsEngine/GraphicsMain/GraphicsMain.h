#pragma once

#include "../Graphics/GraphicsRenderer.h"
#include <vector>
#include <memory>
#include <map>

#include <glew.h>
#include <glfw3.h>

enum class ERerderingTarget
{
	COLOR,
	DEPTH
};

class TheaterDemo;
class CTimeline;
class TransformComponent;

class GraphicsMain
{
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
	bool Initialize();
	bool RunLoop();

	float time;
	float deltaTime;
	std::unique_ptr<CTimeline> m_timeline;
	float animTime;

	//frame board
	std::unique_ptr<class GameObject> renderBoard;
	std::shared_ptr<class CameraObject> game_camera_instance;
	std::map<int, GLenum> texSlots;
	
	//
	TheaterDemo* m_App;
	ERerderingTarget renderingTarget;
	bool isRunning;
	float previousTime;
	std::vector<class GameObject*> gameObjectList;
	std::vector<class GameObject*> raymarchingObjectList;
	std::vector<class GameObject*> boardGameObjectList;
	std::vector<class GameObject*> postProcessGameObjectList;
	std::vector<class UIObject*> uiObjectList;

	bool mouseStateBool;
	std::shared_ptr<TransformComponent> m_CameraTransform;
	
private :
	void UpdateTimeline();
	void InputProcess();
	static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
	void Update();
	void Draw();
	void LoadData();
protected:
	static GraphicsMain* s_pInstance;
};

