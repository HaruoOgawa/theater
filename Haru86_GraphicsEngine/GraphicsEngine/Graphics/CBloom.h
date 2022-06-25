#pragma once
#include <memory>
#include <vector>

class Texture;
class Material;
class Mesh;

class CBloom
{	
	// FrameBuffer
	std::shared_ptr<Texture> m_IlluminanceMap;
	std::shared_ptr<Texture> m_BlurMapX;
	std::shared_ptr<Texture> m_BlurMapY;
	unsigned int m_IlluminanceBuffer;
	unsigned int m_BlurBufferX;
	unsigned int m_BlurBufferY;
	std::vector<float> weights;

	// Mesh Material
	std::shared_ptr<Material> m_IlluminanceMaterial;
	std::shared_ptr<Material> m_BlurMaterial;
	std::shared_ptr<Material> m_BloomResultMaterial;
	std::shared_ptr<Mesh> m_mesh;

public:
	CBloom();
	~CBloom()=default;
	void Initialize();
	void Draw(const std::shared_ptr<Texture> SrcTexture, const unsigned int& DestBuffer);
};

