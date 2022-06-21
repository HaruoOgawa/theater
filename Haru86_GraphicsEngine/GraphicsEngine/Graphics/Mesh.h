#pragma once
#include <vector>
#include <memory>
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

enum class PrimitiveType {
	BOARD,
	CUBE,
	SPHERE,
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
	void Draw(GLDrawType drawType);

	const std::vector<std::shared_ptr<Primitive>>& GetPrimitiveList()const;
	void SetPrimitive(const std::shared_ptr<Primitive>& Primitive);

	friend class Graphics;
	friend class glTFData;
	GLDrawType glDrawType;
private:
	void CreateMesh(PrimitiveType primType);
	std::vector<std::shared_ptr<Primitive>> m_primitives;
};

