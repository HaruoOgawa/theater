#pragma once
#include <memory>

class Texture;
class Mesh;
class Material;

namespace myapp {
	class BillWindowGenerator
	{
		unsigned int                 m_BillWindowTexIndex;
		
		std::shared_ptr<Mesh>        m_BillWindowMesh;
		std::shared_ptr<Material>    m_BillWindowMaterial;
	public:
		BillWindowGenerator();
		virtual ~BillWindowGenerator()=default;

		std::shared_ptr<Texture>     m_BillWindowTex;
	};
}
