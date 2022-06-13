#include "CNode.h"
#include "GraphicsEngine/Component/Component.h"
#include "GraphicsEngine/Component/TransformComponent.h"
#include "GraphicsEngine/Component/MeshRendererComponent.h"
#include "GraphicsEngine/Component/SkinMeshRendererComponent.h"

namespace obj {
	CNode::CNode():
		m_ParentIndex(-1)
	{
		m_transform = std::make_shared<TransformComponent>(nullptr, glm::vec3(0, 0, 0), glm::quat(glm::vec3(0, 0, 0)), glm::vec3(1.0f, 1.0f, 1.0f));
		AddComponent<TransformComponent>(m_transform);
	}

	CNode::~CNode() {
		m_ComponentList.clear();
	}

	//
	bool CNode::Update() {
		for (auto comp : m_ComponentList) {
			comp.second->Update();
		}

		for (auto node : m_Nodes) {
			node->Update();
		}

		return true;
	}

	bool CNode::Draw() {
		m_transform->CalMatrix();

		auto MeshRenderer = GetComponent<MeshRendererComponent>();
		if (MeshRenderer)MeshRenderer->Draw(); // m_ModelMatrixTree‚ð“n‚·

		for (auto node : m_Nodes) {
			// m_ModelMatrixTree‚ðŽq—v‘f‚ÌDraw‚É“n‚·
			node->Draw();
		}

		m_ModelMatrixTree.clear();

		return true;
	}

	bool CNode::Draw(const std::vector<glm::mat4>& ModelMatrixTree) {
		m_transform->CalMatrix();
		m_ModelMatrixTree = ModelMatrixTree;

		//
		m_ModelMatrixTree.push_back(m_transform->GetMMatrix());

		auto MeshRenderer = GetComponent<MeshRendererComponent>();
		if (MeshRenderer)MeshRenderer->Draw(); // m_ModelMatrixTree‚ð“n‚·
		
		auto SkinMeshRenderer = GetComponent<SkinMeshRendererComponent>();
		if (SkinMeshRenderer)SkinMeshRenderer->Draw(m_ModelMatrixTree); // m_ModelMatrixTree‚ð“n‚·

		for (auto node : m_Nodes) {
			// m_ModelMatrixTree‚ðŽq—v‘f‚ÌDraw‚É“n‚·
			node->Draw(m_ModelMatrixTree);
		}

		// m_ModelMatrixTree‚ÌƒŠƒZƒbƒg
		m_ModelMatrixTree.clear();

		return true;
	}

	void CNode::ProcessInput(const std::shared_ptr<app::CEventListener>& EventListener) {
		for (auto comp : m_ComponentList) {
			comp.second->ProcessInput(e);
		}

		for (auto node : m_Nodes) {
			node->ProcessInput(e);
		}
	}

	const std::unordered_map<std::type_index, std::shared_ptr<Component>>& CNode::GetComponentList()const {
		return m_ComponentList;
	}

	const std::shared_ptr<TransformComponent>& CNode::GetTransform()const {
		return m_transform;
	}
	
	std::shared_ptr<TransformComponent>& CNode::GetTransform(){
		return m_transform;
	}

	void CNode::AddNode(const std::shared_ptr<CNode>& node) {
		m_Nodes.push_back(node);
	}

	const std::vector<std::shared_ptr<CNode>>& CNode::GetNodeList()const {
		return m_Nodes;
	}

	const std::shared_ptr<CNode>& CNode::GetNode(int index)const {
		return m_Nodes[index];
	}

	void CNode::SetParentIndex(int ParentIndex) {
		m_ParentIndex = ParentIndex;
	}

	int CNode::GetParentIndex()const {
		return m_ParentIndex;
	}
}