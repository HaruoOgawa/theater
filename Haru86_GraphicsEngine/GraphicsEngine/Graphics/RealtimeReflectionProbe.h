#pragma once
#include <vector>
#include <memory>

class Texture;
class TransformComponent;

class RealtimeReflectionProbe
{
public:
	RealtimeReflectionProbe();
	virtual ~RealtimeReflectionProbe()=default;

	void Start();
	void Update();
	void Draw();

	//
	std::shared_ptr<Texture>                         m_CubeTex;
	unsigned int									 m_FramebufferIndex;
	std::vector<std::shared_ptr<TransformComponent>> m_CubeCameraTRS;
};

