#pragma once
#include <memory>

class Texture;
class Mesh;
class Material;

namespace myapp {
	class ProceduralCity
	{
		unsigned int                 m_BillTexIndex;
		std::shared_ptr<Texture>     m_BillTex;
		std::shared_ptr<Mesh>        m_BillMesh4Instanced;
		std::shared_ptr<Material>    m_BillMaterial4Instanced;

	public:
		ProceduralCity();
		virtual ~ProceduralCity()=default;
		void Start();
		void Update();
		void Draw();
	};
}

