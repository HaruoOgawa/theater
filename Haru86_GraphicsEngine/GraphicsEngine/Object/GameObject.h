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

class ARendererComponent;

namespace physics {
	class CPhysicsEngine;
}

enum RenderQueue
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
	GameObject(PrimitiveType primType);
	GameObject(PrimitiveType primType, std::string vertexShaderName, std::string fragmentShaderName, RenderType renderType);
	
	/*GameObject(PrimitiveType primType, std::map<GLenum, std::string> shaders);
	GameObject(PrimitiveType primType,std::string fragmentShaderName);
	GameObject(PrimitiveType primType, std::string vertexShaderName, std::string fragmentShaderName);
	GameObject(PrimitiveType primType, std::string vertexShaderName, std::string tessellationShaderName[2], std::string fragmentShaderName);*/
	/*GameObject(PrimitiveType primType, std::string vertexShaderName, std::string fragmentShaderName,std::string textureString);*/
	
	GameObject(PrimitiveType primType,
		std::string vert = "",std::string frag = "",std::string geom = "",std::string tc = "",std::string tv="");
	
	~GameObject();
	void UseZTest(bool use);
	void SetRenderOlder(int order);
	void ProcessInput(const std::shared_ptr<app::CEventListener>& EventListener) override;
	std::shared_ptr<ARendererComponent> meshComp;
	float animTime = 0.0f;
	friend class GraphicsRenderer;
	friend class TimelineComponent;
	int renderOrder;
private:
	RenderType renderType;
};

