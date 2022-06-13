#pragma once
#include <memory>
#include <vector>
#include <unordered_map>
#include <typeindex>
#include <typeinfo>

#include <glm/glm.hpp>

class Component;
class TransformComponent;

namespace app {
	class CEventListener;
}

namespace obj {
	class CNode
	{
		std::vector<std::shared_ptr<CNode>> m_Nodes;
		std::shared_ptr<TransformComponent> m_transform;
		std::vector<glm::mat4> m_ModelMatrixTree;
		std::unordered_map<std::type_index, std::shared_ptr<Component>> m_ComponentList;
		int m_ParentIndex;
	public:
		//
		CNode();
		~CNode();

		//
		bool Update();
		bool Draw();
		bool Draw(const std::vector<glm::mat4>& ModelMatrixTree);
		void ProcessInput(const std::shared_ptr<app::CEventListener>& EventListener);

		//
		const std::unordered_map<std::type_index, std::shared_ptr<Component>>& GetComponentList()const;

		const std::shared_ptr<TransformComponent>& GetTransform()const;
		std::shared_ptr<TransformComponent>& GetTransform();

		void AddNode(const std::shared_ptr<CNode>& node);
		const std::vector<std::shared_ptr<CNode>>& GetNodeList()const;
		const std::shared_ptr<CNode>& GetNode(int index)const;

		//
		template<typename T>
		void AddComponent(std::shared_ptr<Component> component) {
			m_ComponentList.insert({ typeid(T),component });
		}

		template<typename T>
		void RemoveComponent() {
			auto item = m_ComponentList.find(typeid(T));
			m_ComponentList.erase(item->first);
		}

		template<typename T>
		T* GetComponent()const {
			auto item = m_ComponentList.find(typeid(T));
			if (item != m_ComponentList.end()) {
				return static_cast<T*>(item->second.get());
			}
			else {
				T* val = nullptr;
				return val;
			}
		}

		void SetParentIndex(int ParentIndex);
		int GetParentIndex()const;
	private:
	};
}
