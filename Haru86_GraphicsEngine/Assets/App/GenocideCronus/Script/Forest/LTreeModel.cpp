#include "LTreeModel.h"
#include "LTree.h"

namespace myapp {
	LTreeModel::LTreeModel() :
		m_LTree(nullptr)
	{
		Start();
	}

	void LTreeModel::Start()
	{
		// 
		m_LTree = std::make_shared<LTree>();
	}

	void LTreeModel::Update()
	{
		m_LTree->Update();
	}

	void LTreeModel::Draw()
	{
		m_LTree->Draw();
	}
}