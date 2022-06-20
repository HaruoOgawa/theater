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

class ARendererComponent;

namespace physics {
	class CPhysicsEngine;
}

enum class RenderQueue
{
	Background=1000,
	Geometry=3000,
	Transparent=4000,
	UI=5000
};

enum class RenderType {
	DefaultBuffer,
	FrameBuffer,
	PostProcess
};

class GameObject
	: public Object
{
	PrimitiveType m_PrimitiveType;
public:
	GameObject(PrimitiveType primType, RenderType renderType=RenderType::DefaultBuffer, RenderQueue renderOrder=RenderQueue::Geometry,
		std::string vert = shaderlib::ShaderLib::Standard_vert,std::string frag = shaderlib::ShaderLib::Standard_frag,std::string geom = "",std::string tc = "",std::string tv="");
	//GameObject(PrimitiveType primType, std::string vertexShaderName, std::string fragmentShaderName, RenderType renderType);

	~GameObject();
	void UseZTest(bool use);
	void SetRenderOlder(int order);
	std::shared_ptr<ARendererComponent> meshComp;
	float animTime = 0.0f;
	friend class GraphicsRenderer;
	friend class TimelineComponent;
	int m_renderOrder;
private:
	RenderType m_renderType;
};

