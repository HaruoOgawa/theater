#pragma once

#include <glew.h>
#include <glfw3.h>

namespace app {
	class CEventListener
	{
		int m_Key;
		int m_mods;
		int m_scancode;
	private:
		void Free();
		static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
	public:
		CEventListener();
		~CEventListener();

		void ListenEvent(GLFWwindow* Window);
	};
}

