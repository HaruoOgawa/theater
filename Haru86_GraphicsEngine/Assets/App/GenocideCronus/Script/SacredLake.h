#pragma once

#include <memory>

class Texture;
class Mesh;
class Material;
class TransformComponent;

namespace myapp {
	class SacredLake
	{
	public:
		SacredLake();
		virtual ~SacredLake() = default;

		void Start();
		void Update();
		void Draw();
	};
}