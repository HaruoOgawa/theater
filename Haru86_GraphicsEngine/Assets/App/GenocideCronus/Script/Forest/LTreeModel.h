#pragma once
#include <memory>

namespace myapp {
	class LTree;
	
	class LTreeModel
	{
		std::shared_ptr<LTree> m_LTree;
	public:
		LTreeModel();
		virtual ~LTreeModel() = default;
		void Start();
		void Update();
		void Draw();
	};
}
