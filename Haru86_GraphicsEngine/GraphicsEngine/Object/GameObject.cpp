#include "../Object/GameObject.h"
#include "../GraphicsMain/GraphicsMain.h"
#include "../Component/TransformComponent.h"
#include "../Component/MeshRendererComponent.h"

GameObject::GameObject(PrimitiveType primType, RenderType renderType, RenderQueue renderOrder,
	std::string vert, std::string frag, std::string geom, std::string tc, std::string tv)
	: Object(), m_renderType(renderType),m_renderOrder(static_cast<int>(renderOrder)), animTime(0.0f), m_PrimitiveType(primType)
{
	//
	if (renderType == RenderType::FrameBuffer) {
		GraphicsMain::GetInstance()->boardGameObjectList.emplace_back(this);
	}
	else if (renderType == RenderType::PostProcess) {
		GraphicsMain::GetInstance()->postProcessGameObjectList.emplace_back(this);
	}
	else {
		GraphicsMain::GetInstance()->gameObjectList.emplace_back(this);
	}

	//
	meshComp =  std::make_shared<MeshRendererComponent>(this, primType, vert, frag, geom, tc, tv);
	AddComponent<MeshRendererComponent>(meshComp);
}

/*GameObject::GameObject(PrimitiveType primType, std::string vertexShaderName, std::string fragmentShaderName, RenderType rt)
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
}*/

GameObject::~GameObject() {
}


void GameObject::UseZTest(bool use) {
	meshComp->SetUseZTest(use);
}

void GameObject::SetRenderOlder(int order) {
	m_renderOrder = order;
}