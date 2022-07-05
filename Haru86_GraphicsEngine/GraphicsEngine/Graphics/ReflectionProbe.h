#pragma once
#include <vector>
#include <memory>

class Texture;
class TransformComponent;

enum class EReflectionType {
	CUBEMAP,
	MONODIRECTIONAL,
};

class ReflectionProbe
{
public:
	ReflectionProbe(EReflectionType ReflectionType);
	virtual ~ReflectionProbe()=default;

	void Start();
	void Update();
	void Draw();

	//
	std::shared_ptr<Texture>                         m_CubeTex;
	unsigned int									 m_FramebufferIndex;
	std::vector<std::shared_ptr<TransformComponent>> m_CubeCameraTRS;
	EReflectionType								     m_ReflectionType;
};

