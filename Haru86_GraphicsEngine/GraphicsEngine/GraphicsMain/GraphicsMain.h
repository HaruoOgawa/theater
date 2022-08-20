#pragma once

#include "../Graphics/GraphicsRenderer.h"
#include <vector>
#include <memory>
#include <map>
#include <glm/glm.hpp>

#include <glew.h>
#include <glfw3.h>

enum class ERerderingTarget
{
	COLOR,
	DEPTH,
	NORMAL,
};

namespace sound {
	class SoundPlayer;
}

class GenocideCronus;
class CTimeline;
class TransformComponent;
class ReflectionProbe;

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

	unsigned int GetAppSceneIndex()const;

	float m_SecondsTime;
	float m_MilliSecondsTime;
	float m_DeltaTime;
	std::unique_ptr<CTimeline> m_timeline;
	float animTime;

	//frame board
	std::unique_ptr<class GameObject> renderBoard;
	std::shared_ptr<class CameraObject> game_camera_instance;
	
	//
	GenocideCronus* m_App;
	ERerderingTarget renderingTarget;
	bool isRunning;
	float previousTime;
	std::vector<class GameObject*> gameObjectList;
	std::vector<class GameObject*> raymarchingObjectList;
	std::vector<class GameObject*> boardGameObjectList;
	std::vector<class GameObject*> postProcessGameObjectList;
	std::vector<class UIObject*> uiObjectList;

	bool mouseStateBool;

	// ライト
	std::shared_ptr<TransformComponent> m_GroabalLightPosition;

	// カメラパラメーター
	//std::vector<std::shared_ptr<TransformComponent>> m_CameraTransformList;
	//int m_UseCameraIndex;

	// カメラ
	std::shared_ptr<TransformComponent> m_MainCamera;
	std::shared_ptr<TransformComponent> m_UsingCamera;

	// 
	std::vector<ReflectionProbe*> m_ReflectionProbeList;

	//
	std::shared_ptr<sound::SoundPlayer> m_SoundPlayer;
	
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

