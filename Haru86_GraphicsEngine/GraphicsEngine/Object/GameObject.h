#pragma once

#include "../GraphicsMain/GraphicsMain.h"
#include "../Graphics/Mesh.h"

#include <vector>
#include <map>
#include <string>
#include <glew.h>
#include <typeinfo>
#include <memory>
#include "GraphicsEngine/Graphics/ShaderLib.h"
#include "GraphicsEngine/Graphics/EGraphicsParam.h"

class MeshRendererComponent;
class TransformComponent;

class GameObject
{
	PrimitiveType m_PrimitiveType;
public:
	GameObject(PrimitiveType primType, RenderType renderType=RenderType::DefaultBuffer, 
		RenderQueue renderOrder=RenderQueue::Geometry, RenderingSurfaceType SurfaceType = RenderingSurfaceType::RASTERIZER,
		std::string vert = shaderlib::ShaderLib::Standard_vert,std::string frag = shaderlib::ShaderLib::Standard_frag,std::string geom = "",std::string tc = "",std::string tv="");

	void SetTransform(glm::vec3 pos, glm::vec3 rot, glm::vec3 s);
	void SetPosition(glm::vec3 pos);
	void SetRotation(glm::vec3 rot);
	void SetScale(glm::vec3 s);
	
	glm::vec3 GetPosition();
	glm::vec3 GetRotation();
	glm::vec3 GetScale();

	void UseZTest(bool use);
	void SetRenderOlder(int order);

	std::shared_ptr<MeshRendererComponent> meshComp;
	std::shared_ptr<TransformComponent> m_transform;

	float animTime = 0.0f;
	friend class GraphicsRenderer;
	int m_renderOrder;
	RenderingSurfaceType m_SurfaceType;
private:
	RenderType m_renderType;
};

