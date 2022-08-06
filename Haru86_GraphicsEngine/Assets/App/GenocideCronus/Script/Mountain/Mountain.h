#pragma once
#include <memory>
#include <string>

class MeshRendererComponent;

namespace myapp {
	class Mountain
	{
		std::shared_ptr<MeshRendererComponent> m_MoutainObj;

	public:
		Mountain();
		virtual ~Mountain() = default;

		void Update();
		void Draw(bool IsRaymarching);
	};
}
