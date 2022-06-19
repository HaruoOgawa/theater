#include "../Object/GameObject.h"
#include "../GraphicsMain/GraphicsMain.h"
#include "../Component/TransformComponent.h"
#include "../Component/ARendererComponent.h"
#include "../Component/MeshRendererComponent.h"
#include "CNode.h"

GameObject::GameObject(PrimitiveType primType) 
	: Object(), renderOrder(RenderQueue::Geometry), renderType(RenderType::DefaultBuffer), animTime(0.0f), m_PrimitiveType(primType)
{
	GraphicsMain::GetInstance()->gameObjectList.emplace_back(this);
	meshComp =  std::make_shared<MeshRendererComponent>(this, primType);
	GetRootNode()->AddComponent<MeshRendererComponent>(meshComp);
}

GameObject::GameObject(PrimitiveType primType, std::string vertexShaderName, std::string fragmentShaderName, RenderType rt)
	:Object(), renderOrder(RenderQueue::UI), renderType(rt), animTime(0.0f), m_PrimitiveType(primType)
{
	if (renderType == RenderType::FrameBuffer) {
		GraphicsMain::GetInstance()->boardGameObjectList.emplace_back(this);
	}
	else if (renderType == RenderType::PostProcess) {
		GraphicsMain::GetInstance()->postProcessGameObjectList.emplace_back(this);
	}

	meshComp = std::make_shared<MeshRendererComponent>(this, primType, vertexShaderName, fragmentShaderName);
	GetRootNode()->AddComponent<MeshRendererComponent>(meshComp);
}

/*GameObject::GameObject(PrimitiveType primType, std::map<GLenum, std::string> shaders)
	: Object(), renderOrder(RenderQueue::Geometry), renderType(RenderType::DefaultBuffer), animTime(0.0f), m_PrimitiveType(primType)
{
	GraphicsMain::GetInstance()->gameObjectList.emplace_back(this);
	meshComp = std::make_shared<MeshRendererComponent>(this, primType,shaders);
	GetRootNode()->AddComponent<MeshRendererComponent>(meshComp);
}

GameObject::GameObject(PrimitiveType primType, std::string fragmentShaderName)
	: Object(), renderOrder(RenderQueue::Geometry), renderType(RenderType::DefaultBuffer), animTime(0.0f), m_PrimitiveType(primType)
{
	GraphicsMain::GetInstance()->gameObjectList.emplace_back(this);
	meshComp = std::make_shared<MeshRendererComponent>(this, primType, "./Assets/Shader/Standard.vert",fragmentShaderName);
	GetRootNode()->AddComponent<MeshRendererComponent>(meshComp);
}

GameObject::GameObject(PrimitiveType primType, std::string vertexShaderName, std::string fragmentShaderName)
	: Object(), renderOrder(RenderQueue::Geometry), renderType(RenderType::DefaultBuffer), animTime(0.0f), m_PrimitiveType(primType)
{
	GraphicsMain::GetInstance()->gameObjectList.emplace_back(this);
	meshComp = std::make_shared<MeshRendererComponent>(this, primType,vertexShaderName,fragmentShaderName);
	GetRootNode()->AddComponent<MeshRendererComponent>(meshComp);
}

GameObject::GameObject(PrimitiveType primType, std::string vertexShaderName, std::string tessellationShaderName[2], std::string fragmentShaderName)
	: Object(), renderOrder(RenderQueue::Geometry), renderType(RenderType::DefaultBuffer), animTime(0.0f), m_PrimitiveType(primType)
{
	GraphicsMain::GetInstance()->gameObjectList.emplace_back(this);
	meshComp = std::make_shared<MeshRendererComponent>(this, primType,vertexShaderName,tessellationShaderName,fragmentShaderName);
	GetRootNode()->AddComponent<MeshRendererComponent>(meshComp);
}*/

/*GameObject::GameObject(PrimitiveType primType, std::string vertexShaderName, std::string fragmentShaderName, std::string textureString)
	: Object(), renderOrder(RenderQueue::UI), renderType(RenderType::DefaultBuffer), animTime(0.0f), m_PrimitiveType(primType)
{
	GraphicsMain::GetInstance()->gameObjectList.emplace_back(this);
	meshComp = std::make_shared<MeshRendererComponent>(this, primType,vertexShaderName,fragmentShaderName, textureString);
	GetRootNode()->AddComponent<MeshRendererComponent>(meshComp);
	UseZTest(false);
	SetRenderOlder(RenderQueue::UI);
}
*/

GameObject::GameObject(PrimitiveType primType,
	std::string vert, std::string frag, std::string geom, std::string tc, std::string tv)
	: Object(), renderOrder(RenderQueue::Geometry), renderType(RenderType::DefaultBuffer), animTime(0.0f), m_PrimitiveType(primType)
{
	GraphicsMain::GetInstance()->gameObjectList.emplace_back(this);
	meshComp =  std::make_shared<MeshRendererComponent>(this, primType, vert, frag, geom, tc, tv);
	GetRootNode()->AddComponent<MeshRendererComponent>(meshComp);
}

GameObject::~GameObject() {
}


void GameObject::UseZTest(bool use) {
	meshComp->SetUseZTest(use);
}

void GameObject::SetRenderOlder(int order) {
	renderOrder = order;
}

void GameObject::ProcessInput(const std::shared_ptr<app::CEventListener>& EventListener) {
	for (auto comp : m_RootNode->GetComponentList()) {
		comp.second->ProcessInput(EventListener);
	}
}
