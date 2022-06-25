#pragma once

#include "../Graphics/Mesh.h"

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
		std::string vert = shaderlib::ShaderLib::Standard_vert,std::string frag = shaderlib::ShaderLib::Standard_frag,std::string geom = "",std::string tc = "",std::string tv="",std::string cs="");

	std::shared_ptr<MeshRendererComponent> meshComp;
	std::shared_ptr<TransformComponent> m_transform;

	float animTime = 0.0f;
	friend class GraphicsRenderer;
	int m_renderOrder;
	RenderingSurfaceType m_SurfaceType;
private:
	RenderType m_renderType;
};

