#pragma once

#include "../GraphicsMain/GraphicsMain.h"
#include "../Component/Component.h"
#include <vector>
#include <unordered_map>
#include <string>
#include <typeinfo>
#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>
#include <memory>
#include <typeindex>

class Component;
class TransformComponent;

class Object
{
public:
	Object();
	virtual ~Object();

	void SetTransform(glm::vec3 pos, glm::vec3 rot, glm::vec3 s);
	void SetPosition(glm::vec3 pos);
	void SetRotation(glm::vec3 rot);
	void SetScale(glm::vec3 s);
	virtual void Update();
	virtual void ProcessInput(const std::shared_ptr<app::CEventListener>& EventListener);
	
	glm::vec3 GetPosition();
	glm::vec3 GetRotation();
	glm::vec3 GetScale();

	//
	const std::unordered_map<std::type_index, std::shared_ptr<Component>>& GetComponentList()const;

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
	
	//
	friend class MeshRendererComponent;
	friend class GraphicsRenderer;
	friend class UIComponent;
	friend class RectTransformComponent;
private:
	class GraphicsMain* game;
protected:
	std::shared_ptr<TransformComponent> m_transform;
	std::unordered_map<std::type_index, std::shared_ptr<Component>> m_ComponentList;
};


