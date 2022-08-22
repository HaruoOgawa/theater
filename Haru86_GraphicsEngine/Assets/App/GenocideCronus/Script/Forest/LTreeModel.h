#pragma once
#include <memory>

class MeshRendererComponent;

namespace myapp {
	class LTreeModel
	{
		std::shared_ptr<MeshRendererComponent> m_TreeRenderer;
	public:
		LTreeModel();
		virtual ~LTreeModel() = default;
		void Start();
		void Update();
		void Draw();
	};
}
