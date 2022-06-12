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

namespace obj {
	class CNode;
}

class Object
{
public:
	Object();
	virtual ~Object();

	void SetTransform(glm::vec3 pos, glm::quat rot, glm::vec3 s);
	void SetPosition(glm::vec3 pos);
	void SetRotation(glm::quat rot);
	void SetScale(glm::vec3 s);
	virtual void Update();
	virtual void ProcessInput(const SDL_Event& e);
	
	glm::vec3 GetPosition();
	glm::quat GetRotation();
	glm::vec3 GetScale();
	
	const std::shared_ptr<obj::CNode>& GetRootNode()const;
	std::shared_ptr<obj::CNode>& GetRootNode();

	friend class MeshRendererComponent;
	friend class GraphicsRenderer;
	friend class UIComponent;
	friend class RectTransformComponent;
private:
	class GraphicsMain* game;
protected:
	std::shared_ptr<obj::CNode> m_RootNode;
};


