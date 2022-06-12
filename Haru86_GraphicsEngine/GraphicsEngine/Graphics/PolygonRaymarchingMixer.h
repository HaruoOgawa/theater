#pragma once
#include <memory>

class Material;
class Mesh;

class PolygonRaymarchingMixer
{
public:
	PolygonRaymarchingMixer();
	~PolygonRaymarchingMixer();
	void Draw();
private:
	std::unique_ptr<Material> m_material;
	std::unique_ptr<Mesh> m_mesh;
};

