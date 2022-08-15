#pragma once
#include <vector>
#include <memory>
#include <glm/glm.hpp>

class Texture;
class TransformComponent;

enum class EReflectionType {
	CUBEMAP,
	MONODIRECTIONAL,
};

enum class EReflectionProbeProgress
{
	Initialize,
	Draw,
	Separation,
	End,
	None,
};

class ReflectionProbe
{
	EReflectionProbeProgress m_Progress;
	int m_RPDrawCount;

	glm::vec3 m_Offset;
	float m_Size;
public:
	ReflectionProbe(glm::vec3 Offset, float Size);
	ReflectionProbe(std::shared_ptr<TransformComponent> TRS);
	virtual ~ReflectionProbe()=default;

	void Start();
	void Update();
	void Draw();

	EReflectionProbeProgress GetRPProgress()const { return m_Progress; }

	//
	std::shared_ptr<Texture>                         m_CubeTex;
	unsigned int									 m_FramebufferIndex;
	std::vector<std::shared_ptr<TransformComponent>> m_CubeCameraTRS;
	EReflectionType								     m_ReflectionType;
};

