#include "../Object/GameObject.h"
#include "../GraphicsMain/GraphicsMain.h"
#include "../Component/TransformComponent.h"
#include "../Component/MeshRendererComponent.h"

GameObject::GameObject(PrimitiveType primType, RenderType renderType, RenderQueue renderOrder,RenderingSurfaceType SurfaceType,
	std::string vert, std::string frag, std::string geom, std::string tc, std::string tv, std::string cs)
	:  m_renderType(renderType),m_renderOrder(static_cast<int>(renderOrder)), animTime(0.0f), m_PrimitiveType(primType), m_SurfaceType(SurfaceType)
{
	//
	if (renderType == RenderType::FrameBuffer) {
		GraphicsMain::GetInstance()->boardGameObjectList.emplace_back(this);
	}
	else if (renderType == RenderType::PostProcess) {
		GraphicsMain::GetInstance()->postProcessGameObjectList.emplace_back(this);
	}
	else {
		if (SurfaceType == RenderingSurfaceType::RASTERIZER) {
			GraphicsMain::GetInstance()->gameObjectList.emplace_back(this);
		}
		else if (SurfaceType == RenderingSurfaceType::RAYMARCHING) {
			GraphicsMain::GetInstance()->raymarchingObjectList.emplace_back(this);
		}
	}

	//
	m_transform = std::make_shared<TransformComponent>(nullptr, glm::vec3(0, 0, 0), glm::vec3(0, 0, 0), glm::vec3(1.0f, 1.0f, 1.0f));
	meshComp =  std::make_shared<MeshRendererComponent>(this, primType, SurfaceType, vert, frag, geom, tc, tv,cs);
}

void GameObject::SetTransform(glm::vec3 pos, glm::vec3 rot, glm::vec3 s) { m_transform->SetPosition(pos); m_transform->SetRotation(rot); m_transform->SetScale(s); m_transform->CalMatrix(); }
void GameObject::SetPosition(glm::vec3 pos) { m_transform->SetPosition(pos); m_transform->CalMatrix(); }
void GameObject::SetRotation(glm::vec3 rot) { m_transform->SetRotation(rot); m_transform->CalMatrix(); }
void GameObject::SetScale(glm::vec3 s) { m_transform->SetScale(s); m_transform->CalMatrix(); }

glm::vec3 GameObject::GetPosition() { return m_transform->GetPosition(); }
glm::vec3 GameObject::GetRotation() { return m_transform->GetRotation(); }
glm::vec3 GameObject::GetScale() { return m_transform->GetScale(); }

void GameObject::UseZTest(bool use) { meshComp->SetUseZTest(use); }
void GameObject::SetRenderOlder(int order) { m_renderOrder = order; }