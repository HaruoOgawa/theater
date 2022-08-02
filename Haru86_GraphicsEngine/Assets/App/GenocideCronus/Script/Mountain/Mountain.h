#pragma once
#include <memory>
#include <string>

class GameObject;

namespace myapp {
	class Mountain
	{
		std::shared_ptr<GameObject> m_MoutainObj;
	public:
		Mountain();
		virtual ~Mountain() = default;
	};
}
