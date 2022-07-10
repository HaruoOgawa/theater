#pragma once

#include <memory>

class Mesh;
class Material;
class TransformComponent;
class GameObject;
class ReflectionProbe;

namespace myapp {
	class SSR_Test
	{
		// 
		std::shared_ptr<GameObject> m_Plane;
		std::shared_ptr<GameObject> m_Obj0;
		std::shared_ptr<GameObject> m_Obj1;
		std::shared_ptr<GameObject> m_Obj2;

		

	public:
		SSR_Test();
		virtual ~SSR_Test() = default;

		void Start();
		void Update();
		void Draw();
	};
}