#pragma once

#include "../GraphicsMain/GraphicsMain.h"
#include "./Object.h"
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

class GameObject
	: public Object
{
	PrimitiveType m_PrimitiveType;
public:
	GameObject(PrimitiveType primType, RenderType renderType=RenderType::DefaultBuffer, 
		RenderQueue renderOrder=RenderQueue::Geometry, RenderingSurfaceType SurfaceType = RenderingSurfaceType::RASTERIZER,
		std::string vert = shaderlib::ShaderLib::Standard_vert,std::string frag = shaderlib::ShaderLib::Standard_frag,std::string geom = "",std::string tc = "",std::string tv="");

	void UseZTest(bool use);
	void SetRenderOlder(int order);
	std::shared_ptr<MeshRendererComponent> meshComp;
	float animTime = 0.0f;
	friend class GraphicsRenderer;
	int m_renderOrder;
	RenderingSurfaceType m_SurfaceType;
private:
	RenderType m_renderType;
};

