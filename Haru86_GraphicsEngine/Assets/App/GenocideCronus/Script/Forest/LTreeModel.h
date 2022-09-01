#pragma once
#include <memory>

class MeshRendererComponent;

namespace myapp {
	class LTreeModel
	{
		
		bool IsStreetLineTree;
	public:
		LTreeModel();
		virtual ~LTreeModel() = default;
		void Start();
		void Update();
		void Draw(int numOfTree = 1024, int LinearInstanceRate = 0, bool UseFade = false);

		void SetIsStreetLineTree(bool IsLine) { IsStreetLineTree = IsLine; }

		std::shared_ptr<MeshRendererComponent> m_TreeRenderer;
	};
}
