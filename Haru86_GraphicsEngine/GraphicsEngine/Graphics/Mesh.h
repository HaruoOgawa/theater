#pragma once
#include <vector>
#include <memory>
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glew.h>

enum class PrimitiveType {
	BOARD,
	POINT,
	SPHERE,
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
	explicit Mesh(PrimitiveType primType);
	//Mesh(std::)
	~Mesh() = default;

	void Draw(GLenum DrawVertexWay= GL_TRIANGLES);
	void DrawInstancedWithMesh(int count, GLenum rendermode);

	GLDrawType glDrawType;
	std::vector<std::shared_ptr<Primitive>> m_primitives;
private:
	void CreateMesh(PrimitiveType primType);
	//void CreateMesh();
};

