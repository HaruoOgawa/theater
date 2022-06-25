#pragma once
#include <vector>
#include <memory>
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

enum class PrimitiveType {
	BOARD,
	POINT,
	CUSTOM,
};

enum class GLDrawType {
	NONE,
	TESSELLATION
};

class Primitive;

class Mesh
{
public:
	Mesh(PrimitiveType primType);
	~Mesh() = default;

	void Draw();
	
	GLDrawType glDrawType;
	std::vector<std::shared_ptr<Primitive>> m_primitives;
private:
	void CreateMesh(PrimitiveType primType);
};

