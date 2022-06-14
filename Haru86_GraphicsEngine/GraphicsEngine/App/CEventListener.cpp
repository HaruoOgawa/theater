#include "CEventListener.h"
#include "GraphicsEngine/GraphicsMain/GraphicsMain.h"

namespace app {
	CEventListener::CEventListener() :
		m_Key(-1),
		m_mods(-1),
		m_scancode(-1)
	{
	}

	CEventListener::~CEventListener() {
		Free();
	}

	void CEventListener::Free() {
		m_Key = -1;
		m_mods = -1;
		m_scancode = -1;
	}

	void CEventListener::ListenEvent(GLFWwindow* Window) {
		Free();
		
		// イベントを発行
		glfwPollEvents();
		// key event
		glfwSetKeyCallback(Window, this->key_callback);
		
	}

	void CEventListener::key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
		if (key == GLFW_KEY_ESCAPE  && action == GLFW_PRESS) {
			GraphicsMain::GetInstance()->SetIsRunning(false);
		}
	}
}